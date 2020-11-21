#include "scapi_messages_asn1c.hpp"

#include "utils.hpp"
#include "ttd_keeper.hpp"
#include "exceptions.hpp"

#include <ScapiSocketRequest.h>
#include <ScapiSocketResponse.h>
#include <ScapiNotification.h>

#include <stdexcept>
#include <memory>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cerrno>

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
        case N_UNINITIALISED: return NexoNokReason_uninitialised;
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
    throw bad_mapping(n, "No valid mapping from NokReason to NexoNokReason");
}

static optional<enum NokReason>
map_nok_reason_from_asn1c(const long int* const n) {
    if (!n) {
        return {};
    }
    switch (*n) {
        case NexoNokReason_none: return N_NONE;
        case NexoNokReason_notImplemented: return N_NOT_IMPLEMENTED;
        case NexoNokReason_originalTrxNotFound: return N_ORIGINAL_TRX_NOT_FOUND;
        case NexoNokReason_technicalError: return N_TECHNICAL_ERROR;
        case NexoNokReason_missingData: return N_MISSING_DATA;
        case NexoNokReason_confError: return N_CONF_ERROR;
        case NexoNokReason_noPermission: return N_NO_PERMISSION;
        case NexoNokReason_configurationError: return N_CONFIGURATION_ERROR;
        case NexoNokReason_amountError: return N_AMOUNT_ERROR;
        case NexoNokReason_kernelError: return N_KERNEL_ERROR;
        case NexoNokReason_dataError: return N_DATA_ERROR;
        case NexoNokReason_noCardInserted: return N_NO_CARD_INSERTED;
        case NexoNokReason_cancelled: return N_CANCELLED;
        case NexoNokReason_aborted: return N_ABORTED;
        case NexoNokReason_timeout: return N_TIMEOUT;
        case NexoNokReason_cardMissing: return N_CARD_MISSING;
        case NexoNokReason_chipError: return N_CHIP_ERROR;
        case NexoNokReason_noProfile: return N_NO_PROFILE;
        case NexoNokReason_fallbackProhibited: return N_FALLBACK_PROHIBITED;
        case NexoNokReason_technologyNotSupported: return N_TECHNOLOGY_NOT_SUPPORTED;
        case NexoNokReason_gpo6985: return N_GPO6985;
        case NexoNokReason_cardBlocked: return N_CARD_BLOCKED;
        case NexoNokReason_emptyList: return N_EMPTY_LIST;
    }
    throw bad_mapping(*n, "No valid mapping from NexoNokReason to NokReason");
}

static optional<enum TerminalErrorReason>
map_error_reason_from_asn1c(const long int* const e) {
    if (!e) {
        return {};
    }
    switch (*e) {
        case NexoTerminalErrorReason_none: return TE_NONE;
        case NexoTerminalErrorReason_hardwareError: return TE_HARDWARE_ERROR;
        case NexoTerminalErrorReason_memoryFailure: return TE_MEMORY_FAILURE;
        case NexoTerminalErrorReason_configurationError: return TE_CONFIGURATION_ERROR;
        case NexoTerminalErrorReason_cryptographicKeysMissing: return TE_CRYPTOGRAPHIC_KEYS_MISSING;
        case NexoTerminalErrorReason_logLimitExceeded: return TE_LOG_LIMIT_EXCEEDED;
        case NexoTerminalErrorReason_communicationError: return TE_COMMUNICATION_ERROR;
        case NexoTerminalErrorReason_unspecified: return TE_UNSPECIFIED;
        case NexoTerminalErrorReason_nexoFastFailure: return TE_NEXO_FAST_FAILURE;
        case NexoTerminalErrorReason_interactionError: return TE_INTERACTION_ERROR;
        case NexoTerminalErrorReason_overspend: return TE_OVERSPEND;
        case NexoTerminalErrorReason_notImplemented: return TER_NOT_IMPLEMENTED;
        case NexoTerminalErrorReason_interfaceContractViolation: return TER_INTERFACE_CONTRACT_VIOLATION;
        case NexoTerminalErrorReason_internalError: return TER_INTERNAL_ERROR;
    }
    throw bad_mapping(*e, "No valid mapping from NexoTerminalErrorReason to TerminalErrorReason");
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
            auto* const tmp = reinterpret_cast<Member*>(calloc(1, sizeof(Member)));
            switch (e.index()) {
            case 0:
                tmp->present = Member_PR_msg;
                tmp->choice.msg = get<0>(e);
                break;
            case 1:
                tmp->present = Member_PR_ssn;
                tmp->choice.ssn = get<1>(e);
                break;
            case 17:
                tmp->present = Member_PR_selectedService;
                tmp->choice.selectedService = get<17>(e);
                break;
            case 18:
                tmp->present = Member_PR_nokReason;
                tmp->choice.nokReason = map_nok_reason_to_asn1c(get<18>(e));
                break;
            default:
                throw runtime_error("Omg"); // FIXME: Memory leak
            }
            if (ASN_SEQUENCE_ADD(&ret.choice.output.what.list, tmp) != 0) {
                // FIXME: Memory leak
                switch (errno) {
                    case EINVAL:
                        throw null_argument({&ret.choice.output.what.list, tmp}, "Can't map Request due to ASN_SEQUENCE_ADD failure");
                    case ENOMEM:
                        throw out_of_memory("Can't map Request due to ASN_SEQUENCE_ADD failure");
                    default:
                        throw 0; // This should never happen
                }
            }
        }
#       if 1
        // Extremely ugly workaround
        const auto l = TtdKeeper::instance().fetch_selected_language();
        ret.choice.output.language = reinterpret_cast<Iso639_t*>(calloc(1, sizeof(Iso639_t)));
        ret.choice.output.language->buf = reinterpret_cast<uint8_t*>(calloc(3, 1));
        ret.choice.output.language->buf[0] = l.c[0];
        ret.choice.output.language->buf[1] = l.c[1];
        ret.choice.output.language->size = 2;
#       endif
        break;
    }
    case 2:
        ret.present = ScapiRequest_PR_print;
        ret.choice.print.type = get<2>(r).type;
        break;
    case 3:
        ret.present = ScapiRequest_PR_entry;
        throw not_implemented("Entry Interaction isn't implemented");
    default:
        throw bad_variant_mapping(r, "Can't encode ScapiRequest"); // FIXME: Memory leak
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
        throw bad_variant_mapping(r, "Can't encode not supported request type");
    }
    unique_ptr<ScapiSocketRequest, asn1c_deleter<&asn_DEF_ScapiSocketRequest>> ret(c);
    if (asn_fprint(stdout, &asn_DEF_ScapiSocketRequest, c) != 0) {
        throw system_error(errno, generic_category(), "asn_DEF_ScapiSocketRequest printing failed");
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
        throw bad_mapping(ptr->rsp.present, "Unexpected response, can't map it internally");
    }
    return ret;
}

static scapi::Nak
map_nak_from_asn1c(const struct ScapiNak& n) {
    return {
        .nokReason = map_nok_reason_from_asn1c(n.nokReason),
        .terminalErrorReason = map_error_reason_from_asn1c(n.terminalErrorReason)
    };
}

static ::scapi::Response
map_nng_from_asn1c(const unique_ptr<ScapiResponse, asn1c_deleter<&asn_DEF_ScapiResponse>>& rsp) {
    const auto ptr = rsp.get();
    switch (ptr->present) {
    case ScapiResponse_PR_ack:
        return scapi::Response(in_place_index<1>);
    case ScapiResponse_PR_nak:
        return map_nak_from_asn1c(ptr->choice.nak);
    case ScapiResponse_PR_ackEntry:
    case ScapiResponse_PR_ackServiceAuthorised:
        throw runtime_error("Not implemented 3");
    case ScapiResponse_PR_NOTHING:
        break;
    }
    throw bad_mapping(ptr->present, "Unexpected ScapiResponse, can't map it internally");
}

static union ExpirationDate
map_expiration_date_from_asn1c(const struct ScapiExpirationDate& d) {
    char f[5];
    if (snprintf(f, sizeof(f), "%02ld%02ld", d.year, d.month) >= integer_cast<int>(sizeof(f))) {
        throw length_error("Expiration date doesn't fit into it's type");
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
                throw length_error("CVD has incorrect length");
            }
            return (struct cn2){ c->choice.cvd.buf[0], c->choice.cvd.buf[1] };
        case CvdData_PR_cvdPresence:
            return static_cast<enum CvdPresence>(c->choice.cvdPresence);
        case CvdData_PR_NOTHING:
        default:
            throw bad_mapping(c->present, "Unexpected CvdData component");
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
        throw bad_mapping(e->present, "Unsupported ScapiEvent");
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
        throw unmet_constraints(errbuf);
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
        throw system_error(errno, generic_category(), "asn_DEF_ScapiSocketRequest printing failed");
    }
    if (RC_OK != r.code) {
        const string str_buf(buf.begin(), buf.end());
        throw WithDecodingData<enum asn_dec_rval_code_e>(r.code, r.consumed, str_buf, "xer_decode failed for ::scapi::socket::Response");
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
    const string str_buf(buf.begin(), buf.end());
    cout << "dec: " << str_buf << endl;
    asn_codec_ctx_t ctx = { };
    ScapiResponse* tmp = NULL;
    const asn_TYPE_descriptor_t* const tp = &asn_DEF_ScapiResponse;
    const asn_dec_rval_t r = xer_decode(&ctx, tp, reinterpret_cast<void**>(&tmp), buf.data(), buf.size());
    unique_ptr<ScapiResponse, asn1c_deleter<&asn_DEF_ScapiResponse>> rsp(tmp);
    if (RC_OK != r.code) {
        throw WithDecodingData<enum asn_dec_rval_code_e>(r.code, r.consumed, str_buf, "xer_decode failed for scapi::Response");
    }
    validate(tp, rsp.get());
    return map_nng_from_asn1c(rsp);
}

::scapi::Notification
decode_nng_ntf(const vector<unsigned char>& buf) {
    const string str_buf(buf.begin(), buf.end());
    cout << "dec: " << str_buf << endl;
    asn_codec_ctx_t ctx = { };
    ScapiNotification* tmp = NULL;
    const asn_TYPE_descriptor_t* const tp = &asn_DEF_ScapiNotification;
    const asn_dec_rval_t r = xer_decode(&ctx, tp, reinterpret_cast<void**>(&tmp), buf.data(), buf.size());
    unique_ptr<ScapiNotification, asn1c_deleter<&asn_DEF_ScapiNotification>> rsp(tmp);
    if (RC_OK != r.code) {
        throw WithDecodingData<enum asn_dec_rval_code_e>(r.code, r.consumed, str_buf, "xer_decode failed for scapi::Response");
    }
    validate(tp, rsp.get());
    return map_nng_ntf_from_asn1c(rsp);
}
