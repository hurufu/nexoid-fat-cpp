#include "ttd_keeper.hpp"
#include "scapi_messages.hpp"
#include "tostring.hpp"

extern "C" {
#include <nexoid/gtd.h>
#include <nexoid/dmapi.h>
}

// FIXME: Remove external dependencies and map exceptions to some internal type
#include <libsocket/exception.hpp>
#include <nngpp/error.h>

#include <iostream>
#include <stdexcept>
#include <cstring>

using namespace std;

static void
set_pan_in_ttd(const string& p) {
    ttd.pan = reinterpret_cast<decltype(ttd.pan)>(dmapi_malloc(sizeof(*ttd.pan)));
    strncpy(*ttd.pan, p.c_str(), sizeof(*ttd.pan));
}

static void
set_expiration_date_in_ttd(const union ExpirationDate& d) {
    ttd.expirationDate = reinterpret_cast<decltype(ttd.expirationDate)>(dmapi_malloc(sizeof(*ttd.expirationDate)));
    *ttd.expirationDate = d;
}

static void
set_cvd_presence_in_ttd(const enum CvdPresence c) {
    ttd.cvdPresence = reinterpret_cast<decltype(ttd.cvdPresence)>(dmapi_malloc(sizeof(*ttd.cvdPresence)));
    *ttd.cvdPresence = c;
}

static void
set_cvd_in_ttd(const struct cn2 c) {
    ttd.cvd = reinterpret_cast<decltype(ttd.cvd)>(dmapi_malloc(sizeof(*ttd.cvd)));
    *ttd.cvd = c;
    set_cvd_presence_in_ttd(CVD_PRESENT);
}

static void
set_cvd_in_ttd(const scapi::CvdData& d) {
    switch (d.index()) {
    case 0:
        set_cvd_presence_in_ttd(get<0>(d));
        break;
    case 1:
        set_cvd_in_ttd(get<1>(d));
        break;
    default:
        throw runtime_error("_");
    }
}

static void
set_manual_entry_in_ttd(const scapi::ManualEntry& m) {
    set_pan_in_ttd(m.pan);
    set_expiration_date_in_ttd(m.expirationDate);
    if (m.cvdData) {
        set_cvd_in_ttd(*m.cvdData);
    }
}

static void
set_error_reason_in_ttd(const enum TerminalErrorReason r) {
    if (ttd.terminalErrorIndicator) {
        if (ttd.terminalErrorReason == TE_NONE) {
            throw runtime_error("Inconsitent error indication 1");
        }
        throw runtime_error("Terminal Error is already set");
    } else {
        ttd.terminalErrorIndicator = true;
        if (ttd.terminalErrorReason != TE_NONE) {
            throw runtime_error("Inconsitent error indication 2");
        }
    }
    ttd.terminalErrorReason = r;
}

static void
set_nok_reason(const enum NokReason n) {
    if (ttd.nokReason != N_NONE) {
        throw runtime_error("Avoided overwritting of nok reason");
    }
    ttd.nokReason = n;
}

static enum IdleEvent
map_event_to_ttd_event_index(const scapi::Event& e) {
    switch (e.index()) {
        case 0: return E_LANGUAGE_SELECTION;
        case 1: return E_SERVICE_SELECTION;
        case 2: return E_MANUAL_ENTRY;
        case 3: return E_TERMINATION_REQUESTED;
        case 4: return E_REBOOT_REQUESTED;
    }
    throw runtime_error("Event can't be mapped");
}

void
TtdKeeper::update(const scapi::Event& e) {
    const auto index = map_event_to_ttd_event_index(e);
    if (ttd.event.Table[index]) {
        throw runtime_error("Event duplicated");
    }
    ttd.event.Table[index] = true;
    switch (index) {
    case E_LANGUAGE_SELECTION:
        ttd.selectedLanguage = get<0>(e).selectedLanguage;
        break;
    case E_SERVICE_SELECTION:
        ttd.selectedService = get<1>(e).serviceId;
        break;
    case E_MANUAL_ENTRY:
        set_manual_entry_in_ttd(get<2>(e));
        break;
    case E_TERMINATION_REQUESTED:
    case E_REBOOT_REQUESTED:
        break;
    default:
        throw runtime_error("Unexpected event");
    }
}

void
TtdKeeper::update(const enum NokReason n) {
    ttd.nokReason = n;
}

void
TtdKeeper::update(const enum AuthorisationResponseCode arc) {
    ttd.authorisationResponseCode = arc;
}

void
TtdKeeper::update(const enum TransactionResult r) {
    if (ttd.transactionResult == T_ABORTED && r != T_ABORTED) {
        char buf[255];
        snprintf(buf, sizeof(buf), "TransactionResult update to %s tried to introduce inconsistency", tostring(r));
        throw runtime_error(buf);
    }
    ttd.transactionResult = r;
}

void
TtdKeeper::handle_bad_response(const scapi::Response& rsp) noexcept {
    if (rsp.index() == 0) { // Nak
        const auto& nak = get<0>(rsp);
        if (nak.nokReason) {
            set_nok_reason(*nak.nokReason);
        }
        if (nak.terminalErrorReason) {
            set_error_reason_in_ttd(*nak.terminalErrorReason);
        }
    } else {
        set_error_reason_in_ttd(TE_INTERACTION_ERROR);
    }
}

void
TtdKeeper::handle_exception(void) noexcept try {
    ttd.terminalErrorIndicator = true;
    throw;
} catch (const nng::exception& e) {
    ttd.terminalErrorReason = TE_COMMUNICATION_ERROR;
    cout << "Messaging related exception originated at '" << e.who() << "' suppressed: " << e.what() << endl;
} catch (const exception& e) {
    ttd.terminalErrorReason = TE_UNSPECIFIED;
    cout << "Generic exception suppressed: " << e.what() << endl;
} catch (const libsocket::socket_exception& e) {
    ttd.terminalErrorReason = TE_COMMUNICATION_ERROR;
    cout << "Connectivity related exception suppressed (" << e.err << ")\n" << e.mesg << endl;
} catch (...) {
    ttd.terminalErrorReason = TE_UNSPECIFIED;
    cout << "Unexpected exception suppressed" << endl;
}

enum NokReason TtdKeeper::fetch_nok_reason(void) {
    return ttd.nokReason;
}

enum TerminalErrorReason TtdKeeper::fetch_ter_reason(void) {
    return ttd.terminalErrorReason;
}

enum ServiceId TtdKeeper::fetch_selected_service(void) {
    return ttd.selectedService;
}

void
TtdKeeper::reset(void) noexcept {
    // FIXME: Implement better clearing method, eg. release ptmalloc arena
    ttd = {};
}
