#include "ttd_keeper.hpp"
#include "scapi_messages.hpp"
#include "tostring.hpp"
#include "exceptions.hpp"

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
        throw duplicated_event(make_desc(index));
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

enum TerminalErrorReason
TtdKeeper::update(const enum TerminalErrorReason t) {
    const auto ret = ttd.terminalErrorReason;
    if (t == TE_NONE) {
        ttd.terminalErrorIndicator = false;
    }
    ttd.terminalErrorReason = t;
    return ret;
}

optional<enum CvdPresence>
TtdKeeper::update(const enum CvdPresence e) {
    const auto old = ttd.cvdPresence;
    set_cvd_presence_in_ttd(e);
    if (old) {
        return *old;
    }
    return {};
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

static TerminalErrorReason
mapto_TerminalErrorReason(const nng::error e) {
    switch (e) {
        case nng::error::success: return TE_NONE;
        case nng::error::nomem: return TE_MEMORY_FAILURE;
        case nng::error::noarg:
        case nng::error::internal:
        case nng::error::inval: return TER_INTERNAL_ERROR;
        case nng::error::timedout: return TER_TIMEOUT;
        case nng::error::notsup: return TER_NOT_IMPLEMENTED;
        case nng::error::syserr: return TER_OS_ERROR;
        default: return TE_COMMUNICATION_ERROR;
    }
}

static TerminalErrorReason
mapto_TerminalErrorReason_for_libsocket(const int e) {
    switch (e) {
        case 0: return TE_NONE;
        case ENOMEM: return TE_MEMORY_FAILURE;
        case EINVAL: return TER_INTERNAL_ERROR;
    }
    return TE_COMMUNICATION_ERROR;
}

void
TtdKeeper::handle_exception(const char* const func) noexcept {
    ttd.terminalErrorIndicator = true;
    ostream& os = cout;
    try {
        if (func) {
            os << func << ": ";
        }
        throw;
    } catch (const exception& e) {
        try {
            throw;
        } catch (const duplicated_event& e) {
            ttd.terminalErrorReason = TER_NOT_IMPLEMENTED;
            os << "Duplicated SCAP event";

        } catch (const not_implemented& e) {
            ttd.terminalErrorReason = TER_NOT_IMPLEMENTED;
            os << "Not implemented";

        } catch (const logic_error& e) {
            ttd.terminalErrorReason = TER_INTERNAL_ERROR;
            try {
                throw;
            } catch (const bad_mapping& e) {
                os << "Bad mapping (" << e.from << ')';
            } catch (const bad_variant_mapping& e) {
                os << "Can't handle variant at " << e.index << " of " << e.tinfo.name();
            } catch (const null_argument& e) {
                os << "Null argument at " << e.null_arg_index;
            } catch (const length_error& e) {
                os << "length_error";
            } catch (const invalid_argument& e) {
                os << "invalid_argument";
            } catch (const domain_error& e) {
                os << "domain_error";
            } catch (const out_of_range& e) {
                os << "out_of_range";
            } catch (...) {
                os << "logic_error";
            }

        } catch (const endec_error& e) {
            ttd.terminalErrorReason = TE_COMMUNICATION_ERROR;
            try {
                throw;
            } catch (const unmet_constraints& e) {
                os << "Message constraints violaiton";
            } catch (const encoding_error& e) {
                os << "Can't encode outgoing message";
            } catch (const decoding_error& e) {
                const string cons(e.get_data_str(), 0, e.consumed);
                os << "Can't decode incomming message (" << e.get_code_str() << "), consumed " << e.consumed << " bytes up to: \"" << cons << '"';
            }
        } catch (const nng::exception& e) {
            ttd.terminalErrorReason = mapto_TerminalErrorReason(e.get_error());
            os << "Messaging error (" << static_cast<int>(e.get_error()) << ") at '" << e.who() << '\'';

        } catch (const system_error& e) {
            ttd.terminalErrorReason = TER_OS_ERROR;
            os << "System Error (" << e.code() << ')';

        } catch (const bad_alloc& e) {
            ttd.terminalErrorReason = TE_MEMORY_FAILURE;
            os << "Out of memory";

        } catch (const bad_typeid& e) {
            ttd.terminalErrorReason = TER_INTERNAL_ERROR;
            os << "bad_typeid";
        } catch (const bad_optional_access& e) {
            ttd.terminalErrorReason = TER_INTERNAL_ERROR;
            os << "bad_optional_access";

        } catch (const runtime_error& e) {
            ttd.terminalErrorReason = TE_UNSPECIFIED;
            os << "runtime_error";

        } catch (...) {
            ttd.terminalErrorReason = TE_UNSPECIFIED;
            os << "Generic Error";
        }

        os << ": " << e.what();

    } catch (const libsocket::socket_exception& e) { // WARNING: They don't inherit std::exception 🤦
        ttd.terminalErrorReason = mapto_TerminalErrorReason_for_libsocket(e.err);
        os << "Socket Error (" << e.err << "): " << e.mesg;
    } catch (...) {
        ttd.terminalErrorReason = TE_UNSPECIFIED;
        os << "...";
    }

    os << endl;
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

union Iso639_1 TtdKeeper::fetch_selected_language(void) {
    return ttd.selectedLanguage;
}

void
TtdKeeper::reset(void) noexcept {
    // FIXME: Implement better clearing method, eg. release ptmalloc arena
    ttd = {};
}
