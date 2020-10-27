#include "scapi_socket_session.hpp"
#include "scapi_nngpp_session.hpp"
#include "scapi_internal.hpp"
#include "scapi_messages_asn1c.hpp"

extern "C" {
#include <nexoid/scapi.h>
#include <nexoid/gtd.h>
#include <nexoid/dmapi.h>
}

// FIXME: Remove external dependencies and map exceptions to some internal type
#include <libsocket/exception.hpp>
#include <nngpp/error.h>

#include <stdexcept>
#include <vector>
#include <iostream>
#include <memory>
#include <cstring>

using namespace std;
using scapi::Session,
      scapi::Request,
      scapi::Response,
      scapi::Interaction,
      scapi::Nak,
      scapi::Event,
      scapi::UpdateInterfaces;

static unique_ptr<Session> s_scapi;

static enum ScapiResult
handle_exception(void) noexcept try {
    ttd.terminalErrorIndicator = true;
    throw;
} catch (const nng::exception& e) {
    ttd.terminalErrorReason = TE_COMMUNICATION_ERROR;
    cout << __FILE__ << ':' << __LINE__ << '@' << __func__ << " Messaging related exception originated at '" << e.who() << "' suppressed: " << e.what() << endl;
    return SCAPI_NOK;
} catch (const exception& e) {
    ttd.terminalErrorReason = TE_UNSPECIFIED;
    cout << __FILE__ << ':' << __LINE__ << '@' << __func__ << " Generic exception suppressed: " << e.what() << endl;
    return SCAPI_NOK;
} catch (const libsocket::socket_exception& e) {
    ttd.terminalErrorReason = TE_COMMUNICATION_ERROR;
    cout << __FILE__ << ':' << __LINE__ << '@' << __func__ << " Connectivity related exception suppressed (" << e.err << ")\n" << e.mesg << endl;
    return SCAPI_NOK;
} catch (...) {
    ttd.terminalErrorReason = TE_UNSPECIFIED;
    cout << __FILE__ << ':' << __LINE__ << '@' << __func__ << " Unexpected exception suppressed" << endl;
    return SCAPI_NOK;
}

static enum ScapiResult
handle_bad_response(const Response& rsp) {
    if (rsp.index() != 0) {
        throw runtime_error("Bad response");
    }
    const auto& nak = get<Nak>(rsp);
    ttd.nokReason = nak.nokReason;
    if (nak.terminalErrorReason) {
        ttd.terminalErrorIndicator = true;
        ttd.terminalErrorReason = *nak.terminalErrorReason;
    }
    return SCAPI_NOK;
}

static int
classify_to_variant_index(const CardholderMessage m) {
    switch (m) {
        case CRDHLDR_ACT_NONE:
        case CRDHLDR_ACT_CLEAR:
        case CRDHLDR_EMV_APPROVED:
        case CRDHLDR_EMV_VOICE_AUTH_REQUIRED:
        case CRDHLDR_EMV_CARD_ERROR:
        case CRDHLDR_EMV_DECLINED:
        case CRDHLDR_EMV_INCORRECT_PIN:
        case CRDHLDR_EMV_INSERT_CARD:
        case CRDHLDR_EMV_PLEASE_WAIT:
        case CRDHLDR_EMV_PROCESSING_ERROR:
        case CRDHLDR_EMV_REMOVE_CARD:
        case CRDHLDR_EMV_USE_CHIP_READER:
        case CRDHLDR_EMV_USE_MAG_STRIPE:
        case CRDHLDR_EMV_TRY_AGAIN:
        case CRDHLDR_MSG_WELCOME:
        case CRDHLDR_MSG_PRESENT_CARD:
        case CRDHLDR_MSG_PROCESSING:
        case CRDHLDR_MSG_CARD_READ_OK_REMOVE_CARD:
        case CRDHLDR_MSG_PLEASE_INSERT_OR_SWIPE_CARD:
        case CRDHLDR_MSG_PLEASE_INSERT_ONE_CARD_ONLY:
        case CRDHLDR_MSG_APPROVED_PLEASE_SIGN:
        case CRDHLDR_MSG_AUTHORISING_PLEASE_WAIT:
        case CRDHLDR_MSG_INSERT_SWIPE_OR_TRY_ANOTHER_CARD:
        case CRDHLDR_MSG_PLEASE_INSERT_CARD:
        case CRDHLDR_MSG_SEE_PHONE_FOR_INSTRUCTIONS:
        case CRDHLDR_MSG_PRESENT_CARD_AGAIN:
        case CRDHLDR_MSG_CARD_WRONG_WAY_OR_NO_CHIP:
        case CRDHLDR_MSG_READ_ERROR:
        case CRDHLDR_MSG_AMOUNT:
        case CRDHLDR_MSG_MAX_AMOUNT:
        case CRDHLDR_MSG_ENTER_PIN:
        case CRDHLDR_MSG_ENTER:
        case CRDHLDR_MSG_AMOUNT_AUTHORISED:
        case CRDHLDR_MSG_LEFT_TO_BE_PAID:
        case CRDHLDR_MSG_TRANSACTION_ABORTED:
        case CRDHLDR_MSG_PAYMENT_APPROVED_CASHBACK_DECLINED:
        case CRDHLDR_MSG_CHIP_ERROR_RE_ENTER_PIN:
        case CRDHLDR_MSG_PRESENT_CARD_OR_USE_MAGSTRIPE:
        case CRDHLDR_MSG_INSERT_OR_PRESENT_CARD:
        case CRDHLDR_MSG_INSERT_OR_SWIPE_CARD:
        case CRDHLDR_MSG_NO_PIN:
        case CRDHLDR_MSG_DIFFERENT_CHOICE:
        case CRDHLDR_MSG_CHOOSE_APPLICATION:
        case CRDHLDR_MSG_AMOUNT_ESTIMATED:
        case CRDHLDR_MSG_FINAL_AMOUNT:
        case CRDHLDR_MSG_AMOUNT_INCREMENT:
        case CRDHLDR_MSG_AMOUNT_DECREMENT:
        case CRDHLDR_MSG_PRINTER_OUT_OF_ORDER:
        case CRDHLDR_MSG_TIP:
        case CRDHLDR_MSG_CASHBACK:
        case CRDHLDR_MSG_PAYMENT:
        case CRDHLDR_MSG_TOTAL:
        case CRDHLDR_ENT_ENTER_PAN:
        case CRDHLDR_ENT_ENTER_EXPIRY_DATE:
        case CRDHLDR_ENT_CVD_PRESENCE:
        case CRDHLDR_ENT_CVD:
        case CRDHLDR_ENT_DCC_CONFIRMATION:
        case CRDHLDR_MSG_REQUEST_SIGNATURE:
        case CRDHLDR_MSG_RECEIPT_PRINTING_FAILED:
        case CRDHLDR_MSG_TERMINAL_MANAGMENT_IN_PROGRESS:
        case CRDHLDR_MSG_FORCE_TRANSACTION_APPROVAL:
            return 0;

        case CRDHLDR_SRC_UI_PARAMETERS_FOR_OUTCOME:
            return -1;
        case CRDHLDR_SRC_UI_PARAMETERS_FOR_RESTART:
            return -1;
        case CRDHLDR_SRC_UI_PARAMETERS_FOR_COMPLETION:
            return -1;
        case CRDHLDR_SRC_DECLINE_DISPLAY_MESSAGE:
            return -1;
        case CRDHLDR_SRC_SELECTED_SERVICE:
            return -1;
        case CRDHLDR_SRC_CARDHOLDER_MESSAGE:
            return -1;
        case CRDHLDR_SRC_NOK_REASON:
            return -1;
        case CRDHLDR_SRC_UI_PARAMETERS_FOR_OUTCOME_STATUS_ONLY:
            return -1;
        case CRDHLDR_SRC_APPLICATION_LABEL_DISPLAYED:
            return -1;
        case CRDHLDR_SRC_PAYMENT_AMOUNT:
            return -1;
        case CRDHLDR_SRC_COMMAND_KEY_ENTER_LABEL:
            return -1;
        case CRDHLDR_SRC_TRX_CURRENCY_ALPHA3:
            return -1;
        case CRDHLDR_SRC_TRX_AMOUNT:
            return -1;
        case CRDHLDR_SRC_COMMAND_KEY_CHANGE_APPLICATION_LABEL:
            return -1;
        case CRDHLDR_SRC_CASHBACK_AMOUNT:
            return -1;
        case CRDHLDR_SRC_SUPPLEMENTARY_AMOUNT:
            return -1;
        case CRDHLDR_SRC_COMMAND_KEY_PIN_BYPASS_LABEL:
            return -1;

        case CRDHLDR_SRC_SEARCH_TRX_RESULT_LIST:
        case CRDHLDR_SRC_UPDATE_PRE_AUTH_TOTAL_AMOUNT:
        case CRDHLDR_SRC_MINUS:
        case CRDHLDR_SSN_CARD_REMOVAL_REQUESTED:
        case CRDHLDR_SSN_CARD_REMOVED:
        case CRDHLDR_SSN_REQUEST_SIGNATURE:
        case CRDHLDR_SSN_RECEIPT_PRINTING_FAILED:
            return 1;
    }
    return -1;
}

static Interaction
map_to_interaction(const CardholderMessage m) {
    Interaction ret;
    switch (classify_to_variant_index(m)) {
    case 0:
        ret.emplace<0>(m);
        break;
    case 1:
        ret.emplace<1>(m);
        break;
    default:
        throw runtime_error("Not implemented");
    }
    return ret;
}

static vector<Interaction>
create_interaction_vector(const size_t size, const CardholderMessage msg[]) {
    vector<Interaction> ret;
    for (size_t i = 0; i < size; i++) {
        ret.push_back(map_to_interaction(msg[0]));
    }
    return ret;
}

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
set_event_in_ttd(const Event& e) {
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

enum ScapiResult
scapi_Initialize(void) noexcept try {
    s_scapi = make_unique<scapi::nngpp::Session>();
    return SCAPI_OK;
} catch (...) {
    return handle_exception();
}

extern "C" enum ScapiResult
scapi_Update_Interfaces(const enum InterfaceStatus status) noexcept try {
    const Request req = (UpdateInterfaces){
        .interfaceStatus = status
    };
    const auto rsp = s_scapi->interaction(req);
    return (rsp.index() == 1) ? SCAPI_OK : handle_bad_response(rsp);
} catch (...) {
    return handle_exception();
}

extern "C" enum ScapiResult
scapi_Data_Print_Interaction(const enum PrintMessage m) noexcept try {
    const Request req = (scapi::PrintMessage){
        .type = m,
        .extraData = nullptr
    };
    const auto rsp = s_scapi->interaction(req);
    return (rsp.index() == 1) ? SCAPI_OK : handle_bad_response(rsp);
} catch (...) {
    return handle_exception();
}

extern "C" enum ScapiResult
scapi_Data_Output_Interaction(const size_t size, const enum CardholderMessage msg[]) noexcept try {
    const Request req(in_place_index<1>, create_interaction_vector(size, msg));
    const auto rsp = s_scapi->interaction(req);
    return (rsp.index() == 1) ? SCAPI_OK : handle_bad_response(rsp);
} catch (...) {
    return handle_exception();
}

extern "C" enum ScapiResult
scapi_Data_Entry_Interaction(size_t size, const enum CardholderMessage msg[]) noexcept try {
    const Request req(in_place_index<3>, create_interaction_vector(size, msg));
    const auto rsp = s_scapi->interaction(req);
    if (rsp.index() != 2) {
        return handle_bad_response(rsp);
    }
    return SCAPI_OK;
} catch (...) {
    return handle_exception();
}

extern "C" enum ScapiResult
scapi_Wait_For_Event(void) noexcept try {
    cout << __func__ << " ..." << endl;
    const auto ntf = s_scapi->notification();
    if (ntf.events.size() == 0) {
        throw runtime_error("Empty event list in SCAP notification isn't supported");
    }
    for (const auto& e : ntf.events) {
        set_event_in_ttd(e);
    }
    return SCAPI_NEW_EVENT;
} catch (...) {
    return handle_exception();
}

extern "C" bool
scapi_Authorise_Service(void) noexcept {
    return true;
}
