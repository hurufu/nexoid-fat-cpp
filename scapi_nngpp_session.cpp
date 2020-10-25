#include "scapi_nngpp_session.hpp"
#include "scapi_messages_asn1c.hpp"

#include <nngpp/nngpp.h>
#include <nngpp/protocol/req0.h>

using namespace ::std;
using namespace ::scapi::nngpp;
using namespace ::nng;
using ::scapi::Response;
using ::scapi::Request;
using ::scapi::Notification;

struct Session::Impl {
    ::socket interaction_socket;

    Impl(void);
    ~Impl(void) = default;

    vector<unsigned char> exch(const vector<unsigned char>&);
    unique_ptr<Response> interaction(const Request& r);
    unique_ptr<Notification> notification(void);
};

Session::Impl::Impl(void) :
    interaction_socket(req::open()) {
    interaction_socket.dial("tcp://localhost:50153");
}

vector<unsigned char>
Session::Impl::exch(const vector<unsigned char>& b) {
    const buffer nreq(b.data(), b.size());
    interaction_socket.send(nreq);
    buffer nrsp = interaction_socket.recv();
    vector<unsigned char> rsp(nrsp.data<unsigned char>(), nrsp.data<unsigned char>() + nrsp.size());
    return rsp;
}

unique_ptr<Response>
Session::Impl::interaction(const Request& r) {
    const auto rq = encode_nng(r);
    const auto rs = exch(rq);
    return make_unique<Response>(decode_nng(rs));
}

unique_ptr<Notification>
Session::Impl::notification(void) {
    return make_unique<Notification>();
}

Session::Session(void) :
    pimpl(make_unique<Session::Impl>()) {
}

Session::~Session(void) = default;

unique_ptr<Response>
Session::interaction(const Request& r) {
    return pimpl->interaction(r);
}

unique_ptr<Notification>
Session::notification(void) {
    return pimpl->notification();
}
