#include "scapi_messages_asn1c.hpp"

#include "utils.hpp"

#include <ScapiSocketRequest.h>
#include <ScapiSocketResponse.h>

#include <stdexcept>
#include <memory>
#include <cstring>
#include <cstdlib>

using namespace std;

template <asn_TYPE_descriptor_t* AsnType>
struct asn1c_deleter {
    void operator()(void* x) {
#       if 0
        ASN_STRUCT_FREE(*AsnType, x);
#       endif
    }
};

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
    ScapiRequest ret;
    switch (r.index()) {
    case 0:
        ret.present = ScapiRequest_PR_updateInterfaces;
        break;
    case 1: {
        ret.present = ScapiRequest_PR_output;
        const size_t ct = get<1>(r).size();
        const size_t sz = ct * sizeof(ScapiInteraction*);
        ret.output.list = {
            .array = reinterpret_cast<ScapiInteraction**>(malloc(sz)),
            .count = integer_cast<int>(ct),
            .size = integer_cast<int>(sz),
            .free = reinterpret_cast<void (*)(ScapiInteraction*&) noexcept>(&free)
        };
        int j = 0;
        for (const auto& e : get<1>(r)) {
            ScapiInteraction* const tmp = reinterpret_cast<ScapiInteraction*>(malloc(sizeof(ScapiInteraction)));
            switch (e.index()) {
            case 0:
                tmp->present = ScapiInteraction_PR_msg;
                tmp->msg = get<0>(e);
                break;
            case 1:
                tmp->present = ScapiInteraction_PR_ssn;
                tmp->msg = get<0>(e);
                break;
            default:
                throw runtime_error("Omg");
            }
            ret.output.list.array[j++] = tmp;
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
        throw runtime_error("Can't encode SCAPI request");
    }
    return ret;
}

static unique_ptr<ScapiSocketRequest, asn1c_deleter<&asn_DEF_ScapiSocketRequest>>
map_to_asn1c(const ::scapi::socket::Request& r) {
    ScapiSocketRequest* const c = reinterpret_cast<ScapiSocketRequest*>(malloc(sizeof(ScapiSocketRequest)));
    switch (r.index()) {
    case 0:
        c->req.present = req_PR_interaction;
        c->req.interaction = map_scapi_request(get<0>(r));
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
        throw runtime_error("Not implemented");
    case rsp_PR_NOTHING:
    default:
        throw runtime_error("Unexpected response, can't map it internally");
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

::scapi::socket::Response
decode(const vector<unsigned char>& buf) {
    asn_codec_ctx_t ctx = { };
    ScapiSocketResponse* tmp = NULL;
    const asn_TYPE_descriptor_t* const tp = &asn_DEF_ScapiSocketResponse;
    const asn_dec_rval_t r = xer_decode(&ctx, tp, reinterpret_cast<void**>(&tmp), buf.data(), buf.size());
    unique_ptr<ScapiSocketResponse, asn1c_deleter<&asn_DEF_ScapiSocketResponse>> rsp(tmp);
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
