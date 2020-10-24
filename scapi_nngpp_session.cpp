#include "scapi_nngpp_session.hpp"

#include <nngpp/nngpp.h>
#include <nngpp/protocol/req0.h>

using namespace ::std;
using namespace ::scapi::nngpp;

struct Session::Impl {
    nng::socket socket;

    Impl(void);
    ~Impl(void) = default;
    nng::buffer exch(const nng::buffer);
};

Session::Impl::Impl(void) :
    socket(nng::req::open()) {
    socket.dial("tcp://localhost:50153");
}

nng::buffer
Session::Impl::exch(const nng::buffer b) {
    socket.send(b);
    return socket.recv();
}

Session::Session(void) :
    pimpl(make_unique<Session::Impl>()) {
}

Session::~Session(void) = default;

unique_ptr<::scapi::Response>
Session::interaction(const ::scapi::Request& r) {
    return make_unique<::scapi::Response>();
}

unique_ptr<::scapi::Notification>
Session::notification(void) {
    return make_unique<::scapi::Notification>();
}
