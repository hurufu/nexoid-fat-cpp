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

    buffer exch(const buffer&);
    unique_ptr<Response> interaction(const Request& r);
    unique_ptr<Notification> notification(void);
};

Session::Impl::Impl(void) :
    interaction_socket(req::open()) {
    interaction_socket.dial("tcp://localhost:50153");
}

buffer
Session::Impl::exch(const buffer& b) {
    interaction_socket.send(b);
    return interaction_socket.recv();
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
