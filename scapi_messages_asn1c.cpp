#include "scapi_messages_asn1c.hpp"

#include <ScapiSocketRequest.h>
#include <ScapiSocketResponse.h>

#include <stdexcept>
#include <memory>

using namespace std;

struct free_delete
{
    void operator()(void* x) { free(x); }
};

static unique_ptr<ScapiSocketRequest, free_delete>
map_to_asn1c(const ::scapi::socket::Request& r) {
    ScapiSocketRequest* const c = reinterpret_cast<ScapiSocketRequest*>(malloc(sizeof(ScapiSocketRequest)));
    switch (r.index()) {
    case 0:
        c->req.present = req_PR_interaction;
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
    unique_ptr<ScapiSocketRequest, free_delete> ret(c);
    return ret;
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
    const auto c = map_to_asn1c(r);
    vector<unsigned char> ret;
    const struct asn_enc_rval_s res = xer_encode(&asn_DEF_ScapiSocketRequest, c.get(), XER_F_CANONICAL, &consume_bytes_cb, &ret);
    if (res.encoded < 0) {
        throw runtime_error("Can't encode using XER");
    }

    return ret;
}

const ::scapi::socket::Response&
decode(const vector<unsigned char>&) {
    throw runtime_error("Not implemented");
}
