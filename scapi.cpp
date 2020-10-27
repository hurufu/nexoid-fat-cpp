#include "scapi_socket_session.hpp"
#include "scapi_nngpp_session.hpp"
#include "scapi_internal.hpp"
#include "scapi_messages_asn1c.hpp"
#include "ttd_keeper.hpp"
#include "tostring.hpp"

extern "C" {
#include <nexoid/scapi.h>
}

#include <stdexcept>
#include <vector>
#include <iostream>
#include <memory>
#include <cstring>

using namespace std;
using scapi::Session,
      scapi::Request,
      scapi::Interaction,
      scapi::UpdateInterfaces;

static unique_ptr<Session> s_scapi;

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
            return 2;
        case CRDHLDR_SRC_UI_PARAMETERS_FOR_RESTART:
            return 3;
        case CRDHLDR_SRC_UI_PARAMETERS_FOR_COMPLETION:
            return 4;
        case CRDHLDR_SRC_DECLINE_DISPLAY_MESSAGE:
            return 6;
        case CRDHLDR_SRC_SELECTED_SERVICE:
            return 17;
        case CRDHLDR_SRC_CARDHOLDER_MESSAGE:
            return 7;
        case CRDHLDR_SRC_NOK_REASON:
            return 18;
        case CRDHLDR_SRC_UI_PARAMETERS_FOR_OUTCOME_STATUS_ONLY:
            return 5;
        case CRDHLDR_SRC_APPLICATION_LABEL_DISPLAYED:
            return 8;
        case CRDHLDR_SRC_PAYMENT_AMOUNT:
            return 12;
        case CRDHLDR_SRC_COMMAND_KEY_ENTER_LABEL:
            return 9;
        case CRDHLDR_SRC_TRX_CURRENCY_ALPHA3:
            return 16;
        case CRDHLDR_SRC_TRX_AMOUNT:
            return 13;
        case CRDHLDR_SRC_COMMAND_KEY_CHANGE_APPLICATION_LABEL:
            return 10;
        case CRDHLDR_SRC_CASHBACK_AMOUNT:
            return 14;
        case CRDHLDR_SRC_SUPPLEMENTARY_AMOUNT:
            return 15;
        case CRDHLDR_SRC_COMMAND_KEY_PIN_BYPASS_LABEL:
            return 11;
        case CRDHLDR_SRC_SEARCH_TRX_RESULT_LIST:
            return 19;
        case CRDHLDR_SRC_UPDATE_PRE_AUTH_TOTAL_AMOUNT:
            return 20;
        case CRDHLDR_SRC_MINUS:
            return 21;
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
    case 17:
        ret.emplace<17>(TtdKeeper::instance().fetch_selected_service());
        break;
    case 18:
        ret.emplace<18>(TtdKeeper::instance().fetch_nok_reason());
        break;
    default: {
        char buf[255];
        snprintf(buf, sizeof(buf), "Not implemented mapping [%x] %s", m, tostring(m));
        throw runtime_error(buf);
    }}
    return ret;
}

static vector<Interaction>
create_interaction_vector(const size_t size, const CardholderMessage msg[]) {
    vector<Interaction> ret;
    for (size_t i = 0; i < size; i++) {
        ret.push_back(map_to_interaction(msg[i]));
    }
    return ret;
}

enum ScapiResult
scapi_Initialize(void) noexcept try {
    s_scapi = make_unique<scapi::nngpp::Session>();
    return SCAPI_OK;
} catch (...) {
    TtdKeeper::instance().handle_exception();
    return SCAPI_NOK;
}

extern "C" enum ScapiResult
scapi_Update_Interfaces(const enum InterfaceStatus status) noexcept try {
    const Request req = (UpdateInterfaces){
        .interfaceStatus = status
    };
    const auto rsp = s_scapi->interaction(req);
    if (rsp.index() == 1) {
        return SCAPI_OK;
    } else {
        TtdKeeper::instance().handle_bad_response(rsp);
        return SCAPI_NOK;
    }
} catch (...) {
    TtdKeeper::instance().handle_exception();
    return SCAPI_NOK;
}

extern "C" enum ScapiResult
scapi_Data_Print_Interaction(const enum PrintMessage m) noexcept try {
    const Request req = (scapi::PrintMessage){
        .type = m,
        .extraData = nullptr
    };
    const auto rsp = s_scapi->interaction(req);
    if (rsp.index() == 1) {
        return SCAPI_OK;
    } else {
        TtdKeeper::instance().handle_bad_response(rsp);
        return SCAPI_NOK;
    }
} catch (...) {
    TtdKeeper::instance().handle_exception();
    return SCAPI_NOK;
}

extern "C" enum ScapiResult
scapi_Data_Output_Interaction(const size_t size, const enum CardholderMessage msg[]) noexcept try {
    const Request req(in_place_index<1>, create_interaction_vector(size, msg));
    const auto rsp = s_scapi->interaction(req);
    if (rsp.index() == 1) {
        return SCAPI_OK;
    } else {
        TtdKeeper::instance().handle_bad_response(rsp);
        return SCAPI_NOK;
    }
} catch (...) {
    TtdKeeper::instance().handle_exception();
    return SCAPI_NOK;
}

extern "C" enum ScapiResult
scapi_Data_Entry_Interaction(size_t size, const enum CardholderMessage msg[]) noexcept try {
    const Request req(in_place_index<3>, create_interaction_vector(size, msg));
    const auto rsp = s_scapi->interaction(req);
    if (rsp.index() != 2) {
        TtdKeeper::instance().handle_bad_response(rsp);
        return SCAPI_NOK;
    }
    return SCAPI_OK;
} catch (...) {
    TtdKeeper::instance().handle_exception();
    return SCAPI_NOK;
}

extern "C" enum ScapiResult
scapi_Wait_For_Event(void) noexcept try {
    cout << __func__ << " ..." << endl;
    const auto ntf = s_scapi->notification();
    for (const auto& e : ntf.events) {
        TtdKeeper::instance().update(e);
    }
    return SCAPI_NEW_EVENT;
} catch (...) {
    TtdKeeper::instance().handle_exception();
    return SCAPI_NOK;
}

extern "C" bool
scapi_Authorise_Service(void) noexcept {
    return true;
}
