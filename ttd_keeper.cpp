#include "ttd_keeper.hpp"
#include "scapi_messages.hpp"

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

void
TtdKeeper::update(const scapi::Event& e) {
    switch (e.index()) {
    case 0:
        ttd.event.Table[E_LANGUAGE_SELECTION] = true;
        ttd.selectedLanguage = get<0>(e).selectedLanguage;
        break;
    case 1:
        ttd.event.Table[E_SERVICE_SELECTION] = true;
        ttd.selectedService = get<1>(e).serviceId;
        break;
    case 2:
        ttd.event.Table[E_MANUAL_ENTRY] = true;
        set_manual_entry_in_ttd(get<2>(e));
        break;
    case 3:
        ttd.event.Table[E_TERMINATION_REQUESTED] = true;
        break;
    case 4:
        ttd.event.Table[E_REBOOT_REQUESTED] = true;
        break;
    default:
        throw runtime_error("Unexpected event");
    }
}

void
TtdKeeper::handle_bad_response(const scapi::Response& rsp) {
    if (rsp.index() != 0) {
        throw runtime_error("Bad response");
    }
    const auto& nak = get<scapi::Nak>(rsp);
    ttd.nokReason = nak.nokReason;
    if (nak.terminalErrorReason) {
        ttd.terminalErrorIndicator = true;
        ttd.terminalErrorReason = *nak.terminalErrorReason;
    }
}

void
TtdKeeper::handle_exception(void) noexcept try {
    ttd.terminalErrorIndicator = true;
    throw;
} catch (const nng::exception& e) {
    ttd.terminalErrorReason = TE_COMMUNICATION_ERROR;
    cout << __FILE__ << ':' << __LINE__ << '@' << __func__ << " Messaging related exception originated at '" << e.who() << "' suppressed: " << e.what() << endl;
} catch (const exception& e) {
    ttd.terminalErrorReason = TE_UNSPECIFIED;
    cout << __FILE__ << ':' << __LINE__ << '@' << __func__ << " Generic exception suppressed: " << e.what() << endl;
} catch (const libsocket::socket_exception& e) {
    ttd.terminalErrorReason = TE_COMMUNICATION_ERROR;
    cout << __FILE__ << ':' << __LINE__ << '@' << __func__ << " Connectivity related exception suppressed (" << e.err << ")\n" << e.mesg << endl;
} catch (...) {
    ttd.terminalErrorReason = TE_UNSPECIFIED;
    cout << __FILE__ << ':' << __LINE__ << '@' << __func__ << " Unexpected exception suppressed" << endl;
}

template <>
enum NokReason& TtdKeeper::fetch<enum NokReason>(void) {
    return ttd.nokReason;
}

template <>
enum TerminalErrorReason& TtdKeeper::fetch<enum TerminalErrorReason>(void) {
    return ttd.terminalErrorReason;
}

void
TtdKeeper::reset(void) noexcept {
    // FIXME: Implement better clearing method, eg. release ptmalloc arena
    ttd = {};
}
