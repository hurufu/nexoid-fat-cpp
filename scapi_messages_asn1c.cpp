#include "scapi_messages_asn1c.hpp"

#include "utils.hpp"

#include <ScapiSocketRequest.h>
#include <ScapiSocketResponse.h>
#include <ScapiNotification.h>

#include <stdexcept>
#include <memory>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>

// TODO: Redesign module scapi_messages_asn1c to be more universal

using namespace std;

template <asn_TYPE_descriptor_t* AsnType>
struct asn1c_deleter {
    void operator()(void* x) {
        ASN_STRUCT_FREE(*AsnType, x);
    }
};

static enum NexoNokReason
map_nok_reason_to_asn1c(const enum NokReason n) {
    switch (n) {
        case N_NONE: return NexoNokReason_none;
        case N_NOT_IMPLEMENTED: return NexoNokReason_notImplemented;
        case N_ORIGINAL_TRX_NOT_FOUND: return NexoNokReason_originalTrxNotFound;
        case N_TECHNICAL_ERROR: return NexoNokReason_technicalError;
        case N_MISSING_DATA: return NexoNokReason_missingData;
        case N_CONF_ERROR: return NexoNokReason_confError;
        case N_NO_PERMISSION: return NexoNokReason_noPermission;
        case N_CONFIGURATION_ERROR: return NexoNokReason_confError;
        case N_AMOUNT_ERROR: return NexoNokReason_amountError;
        case N_KERNEL_ERROR: return NexoNokReason_kernelError;
        case N_DATA_ERROR: return NexoNokReason_dataError;
        case N_NO_CARD_INSERTED: return NexoNokReason_noCardInserted;
        case N_CANCELLED: return NexoNokReason_cancelled;
        case N_ABORTED: return NexoNokReason_aborted;
        case N_TIMEOUT: return NexoNokReason_timeout;
        case N_CARD_MISSING: return NexoNokReason_cardMissing;
        case N_CHIP_ERROR: return NexoNokReason_chipError;
        case N_NO_PROFILE: return NexoNokReason_noProfile;
        case N_FALLBACK_PROHIBITED: return NexoNokReason_fallbackProhibited;
        case N_TECHNOLOGY_NOT_SUPPORTED: return NexoNokReason_technologyNotSupported;
        case N_GPO6985: return NexoNokReason_gpo6985;
        case N_CARD_BLOCKED: return NexoNokReason_cardBlocked;
        case N_EMPTY_LIST: return NexoNokReason_emptyList;
        case N_MAX:
            break;
    }
    throw runtime_error("Invalid NokReason conversion");
}

static const char*
asn_dec_rval_code_e_tostring(const asn_dec_rval_code_e code) {
    switch (code) {
        case RC_OK: return "RC_OK";
        case RC_WMORE: return "RC_WMORE";
        case RC_FAIL: return "RC_FAIL";
    }
    return NULL;
}

static ScapiRequest
map_scapi_request(const ::scapi::Request& r) {
    ScapiRequest ret = { };
    switch (r.index()) {
    case 0:
        ret.present = ScapiRequest_PR_updateInterfaces;
        ret.choice.updateInterfaces.interfaceStatus.buf = reinterpret_cast<uint8_t*>(calloc(1, 1));
        ret.choice.updateInterfaces.interfaceStatus.size = 1;
        ret.choice.updateInterfaces.interfaceStatus.buf[0] = get<0>(r).interfaceStatus;
        break;
    case 1: {
        ret.present = ScapiRequest_PR_output;
        for (const auto& e : get<1>(r)) {
            ScapiInteraction* const tmp = reinterpret_cast<ScapiInteraction*>(calloc(1, sizeof(ScapiInteraction)));
            switch (e.index()) {
            case 0:
                tmp->present = ScapiInteraction_PR_msg;
                tmp->choice.msg = get<0>(e);
                break;
            case 1:
                tmp->present = ScapiInteraction_PR_ssn;
                tmp->choice.ssn = get<1>(e);
                break;
            case 17:
                tmp->present = ScapiInteraction_PR_selectedService;
                tmp->choice.selectedService = get<17>(e);
                break;
            case 18:
                tmp->present = ScapiInteraction_PR_nokReason;
                tmp->choice.nokReason = map_nok_reason_to_asn1c(get<18>(e));
                break;
            default:
                throw runtime_error("Omg"); // FIXME: Memory leak
            }
            if (ASN_SEQUENCE_ADD(&ret.choice.output, tmp) != 0) {
                throw runtime_error("ASN_SEQUENCE_ADD failed"); // FIXME: Memory leak
            }
        }
        break;
    }
    case 2:
        ret.present = ScapiRequest_PR_print;
        break;
    case 3:
        ret.present = ScapiRequest_PR_entry;
        break;
    default:
        throw runtime_error("Can't encode SCAPI request"); // FIXME: Memory leak
    }
    return ret;
}

static unique_ptr<ScapiSocketRequest, asn1c_deleter<&asn_DEF_ScapiSocketRequest>>
map_to_asn1c(const ::scapi::socket::Request& r) {
    ScapiSocketRequest* const c = reinterpret_cast<ScapiSocketRequest*>(calloc(1, sizeof(ScapiSocketRequest)));
    switch (r.index()) {
    case 0:
        c->req.present = req_PR_interaction;
        c->req.choice.interaction = map_scapi_request(get<0>(r));
        break;
    case 1:
        c->req.present = req_PR_registration;
        break;
    case 2:
        c->req.present = req_PR_notification;
        break;
    default:
        throw runtime_error("Can't encode not supported request type");
    }
    unique_ptr<ScapiSocketRequest, asn1c_deleter<&asn_DEF_ScapiSocketRequest>> ret(c);
    if (asn_fprint(stdout, &asn_DEF_ScapiSocketRequest, c) != 0) {
        throw runtime_error("asn_DEF_ScapiSocketRequest printing failed");
    }
    return ret;
}

static ::scapi::socket::Response
map_from_asn1c(const unique_ptr<ScapiSocketResponse, asn1c_deleter<&asn_DEF_ScapiSocketResponse>>& rsp) {
    const auto ptr = rsp.get();
    ::scapi::socket::Response ret;
    switch (ptr->rsp.present) {
    case rsp_PR_registration:
        ret.emplace<1>();
        break;
    case rsp_PR_interaction:
    case rsp_PR_notification:
        throw runtime_error("Not implemented 2");
    case rsp_PR_NOTHING:
    default:
        throw runtime_error("Unexpected response, can't map it internally");
    }
    return ret;
}

static ::scapi::Response
map_nng_from_asn1c(const unique_ptr<ScapiResponse, asn1c_deleter<&asn_DEF_ScapiResponse>>& rsp) {
    const auto ptr = rsp.get();
    ::scapi::Response ret;
    switch (ptr->present) {
    case ScapiResponse_PR_ack:
        ret.emplace<1>();
        break;
    case ScapiResponse_PR_nak:
    case ScapiResponse_PR_ackEntry:
    case ScapiResponse_PR_ackServiceAuthorised:
        throw runtime_error("Not implemented 3");
    case ScapiResponse_PR_NOTHING:
    default:
        throw runtime_error("Unexpected response, can't map it internally");
    }
    return ret;
}

static union ExpirationDate
map_expiration_date_from_asn1c(const struct ScapiExpirationDate& d) {
    char f[5];
    if (snprintf(f, sizeof(f), "%02ld%02ld", d.year, d.month) >= integer_cast<int>(sizeof(f))) {
        throw runtime_error("Expiration date doesn't fit into it's type");
    }
    return { .full = { f[0], f[1], f[2], f[3] } };
}

static optional<scapi::CvdData>
map_cvd_data_from_asn1c(const struct CvdData* const c) {
    if (!c) {
        return {};
    }
    switch (c->present) {
        case CvdData_PR_cvd:
            if (c->choice.cvd.size != 2) {
                throw runtime_error("CVD has incorrect length");
            }
            return (struct cn2){ c->choice.cvd.buf[0], c->choice.cvd.buf[1] };
        case CvdData_PR_cvdPresence:
            return static_cast<enum CvdPresence>(c->choice.cvdPresence);
        case CvdData_PR_NOTHING:
        default:
            throw runtime_error("Unexpected CvdData component");
    }
}

static scapi::ManualEntry
map_manual_entry_from_asn1c(const struct ScapiEventManualEntry& m) {
    return {
        .pan = string(reinterpret_cast<char*>(m.pan.buf), m.pan.size),
        .expirationDate = map_expiration_date_from_asn1c(m.expirationDate),
        .cvdData = map_cvd_data_from_asn1c(m.cvdData)
    };
}

static scapi::Event
map_event_from_asn1c(const struct ScapiEvent* const e) {
    switch (e->present) {
    case ScapiEvent_PR_languageSelection: {
        const auto l = reinterpret_cast<char*>(e->choice.languageSelection.language.buf);
        const scapi::LanguageSelection ls{
            .selectedLanguage = { .c = { l[0], l[1] } }
        };
        return {ls};
    }
    case ScapiEvent_PR_serviceSelection: {
        const scapi::ServiceSelection ss{
            .serviceId = static_cast<enum ServiceId>(e->choice.serviceSelection.serviceId)
        };
        return {ss};
    }
    case ScapiEvent_PR_manualEntry:
        return map_manual_entry_from_asn1c(e->choice.manualEntry);
    case ScapiEvent_PR_terminate:
        return scapi::Event(in_place_index<3>);
    case ScapiEvent_PR_reboot:
        return scapi::Event(in_place_index<4>);
    case ScapiEvent_PR_NOTHING:
    default:
        throw runtime_error("Unsupported");
    }
}

static ::scapi::Notification
map_nng_ntf_from_asn1c(const unique_ptr<ScapiNotification, asn1c_deleter<&asn_DEF_ScapiNotification>>& rsp) {
    ::scapi::Notification ret;
    for (int i = 0; i < rsp->events.list.count; i++) {
        const auto tmp = rsp->events.list.array[i];
        const auto evt = map_event_from_asn1c(tmp);
        ret.events.push_back(evt);
    }
    return ret;
}

static void
validate(const asn_TYPE_descriptor_t* const tp, const void* const rsp) {
    char errbuf[255];
    size_t errlen = sizeof(errbuf);
    if (asn_check_constraints(tp, rsp, errbuf, &errlen) != 0) {
        char b[350];
        snprintf(b, sizeof(b), "Constraints validation failed: %s", errbuf);
        throw runtime_error(errbuf);
    }
}

static int
consume_bytes(const void* const b, const size_t s, vector<unsigned char>* const dst) {
    const vector<unsigned char> tmp(reinterpret_cast<const unsigned char*>(b), reinterpret_cast<const unsigned char*>(b) + s);
    std::move(tmp.begin(), tmp.end(), std::back_inserter(*dst));
    return 0;
}

/** Only needed for casting
 */
static int
consume_bytes_cb(const void* const b, const size_t s, void* const dst) {
    return consume_bytes(b, s, reinterpret_cast<vector<unsigned char>*>(dst));
}

vector<unsigned char>
encode(const ::scapi::socket::Request& r) {
    static const asn_TYPE_descriptor_t* const tp = &asn_DEF_ScapiSocketRequest;
    const auto c = map_to_asn1c(r);
    validate(tp, c.get());
    vector<unsigned char> ret;
    const struct asn_enc_rval_s res = xer_encode(tp, c.get(), XER_F_CANONICAL, &consume_bytes_cb, &ret);
    if (res.encoded < 0) {
        throw runtime_error("Can't encode using XER");
    }

    return ret;
}

vector<unsigned char>
encode_nng(const ::scapi::Request& r) {
    static const auto tp = &asn_DEF_ScapiRequest;
    auto c = map_scapi_request(r);
    validate(tp, &c);
    vector<unsigned char> ret;
    const auto res = xer_encode(tp, &c, XER_F_CANONICAL, &consume_bytes_cb, &ret);
    ASN_STRUCT_RESET(*tp, &c);
    if (res.encoded < 0) {
        throw runtime_error("Can't encode using XER");
    }
    cout << "enc: " << string(ret.begin(), ret.end()) << endl;
    return ret;
}

::scapi::socket::Response
decode(const vector<unsigned char>& buf) {
    asn_codec_ctx_t ctx = { };
    ScapiSocketResponse* tmp = NULL;
    const asn_TYPE_descriptor_t* const tp = &asn_DEF_ScapiSocketResponse;
    const asn_dec_rval_t r = xer_decode(&ctx, tp, reinterpret_cast<void**>(&tmp), buf.data(), buf.size());
    unique_ptr<ScapiSocketResponse, asn1c_deleter<&asn_DEF_ScapiSocketResponse>> rsp(tmp);
    cout << "Received: \"" << string(buf.begin(), buf.end()) << '"' << endl;
    if (asn_fprint(stdout, tp, tmp) != 0) {
        throw runtime_error("asn_DEF_ScapiSocketRequest printing failed");
    }
    if (RC_OK != r.code) {
        char buf[255];
        snprintf(buf, sizeof(buf), "xer_decode returned: { code: %s, consumed: %zu }", asn_dec_rval_code_e_tostring(r.code), r.consumed);
        throw runtime_error(buf);
    }
#   if 0
    if (r.consumed != buf.size()) {
        throw runtime_error("Received too much data");
    }
#   endif
    validate(tp, rsp.get());
    return map_from_asn1c(rsp);
}

::scapi::Response
decode_nng(const vector<unsigned char>& buf) {
    cout << "dec: " << string(buf.begin(), buf.end()) << endl;
    asn_codec_ctx_t ctx = { };
    ScapiResponse* tmp = NULL;
    const asn_TYPE_descriptor_t* const tp = &asn_DEF_ScapiResponse;
    const asn_dec_rval_t r = xer_decode(&ctx, tp, reinterpret_cast<void**>(&tmp), buf.data(), buf.size());
    unique_ptr<ScapiResponse, asn1c_deleter<&asn_DEF_ScapiResponse>> rsp(tmp);
    if (RC_OK != r.code) {
        char err[255];
        snprintf(err, sizeof(err), "xer_decode returned: { code: %s, consumed: %zu, up-to: \"%.*s\" }",
                asn_dec_rval_code_e_tostring(r.code), r.consumed, integer_cast<int>(r.consumed), buf.data());
        throw runtime_error(err);
    }
    validate(tp, rsp.get());
    return map_nng_from_asn1c(rsp);
}

::scapi::Notification
decode_nng_ntf(const vector<unsigned char>& buf) {
    cout << "dec: " << string(buf.begin(), buf.end()) << endl;
    asn_codec_ctx_t ctx = { };
    ScapiNotification* tmp = NULL;
    const asn_TYPE_descriptor_t* const tp = &asn_DEF_ScapiNotification;
    const asn_dec_rval_t r = xer_decode(&ctx, tp, reinterpret_cast<void**>(&tmp), buf.data(), buf.size());
    unique_ptr<ScapiNotification, asn1c_deleter<&asn_DEF_ScapiNotification>> rsp(tmp);
    if (RC_OK != r.code) {
        char err[255];
        snprintf(err, sizeof(err), "xer_decode returned: { code: %s, consumed: %zu, up-to: \"%.*s\" }",
                asn_dec_rval_code_e_tostring(r.code), r.consumed, integer_cast<int>(r.consumed), buf.data());
        throw runtime_error(err);
    }
    validate(tp, rsp.get());
    return map_nng_ntf_from_asn1c(rsp);
}
