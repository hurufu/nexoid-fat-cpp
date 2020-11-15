#include "scapi_nngpp_session.hpp"
#include "scapi_messages_asn1c.hpp"

#include <nngpp/nngpp.h>
#include <nngpp/protocol/req0.h>
#include <nngpp/protocol/pull0.h>

using namespace ::std;
using namespace ::scapi::nngpp;
using namespace ::nng;
using ::scapi::Response;
using ::scapi::Request;
using ::scapi::Notification;

struct Session::Impl {
    ::socket interaction_socket;
    ::socket notification_socket;

    Impl(void);
    ~Impl(void) = default;

    vector<unsigned char> exch(const vector<unsigned char>&);
    Response interaction(const Request& r);
    Notification notification(void);
};

Session::Impl::Impl(void) :
    interaction_socket(req::open()),
    notification_socket(pull::open()) {
    set_opt_recv_timeout(interaction_socket, 3 * 1000);
    set_opt_send_timeout(interaction_socket, 1 * 1000);
    interaction_socket.dial("ipc:///tmp/fatrq");
    notification_socket.dial("ipc:///tmp/fatnt");
}

vector<unsigned char>
Session::Impl::exch(const vector<unsigned char>& b) {
    const view vreq(b.data(), b.size());
    interaction_socket.send(vreq);
    buffer nrsp = interaction_socket.recv();
    vector<unsigned char> rsp(nrsp.data<unsigned char>(), nrsp.data<unsigned char>() + nrsp.size());
    return rsp;
}

Response
Session::Impl::interaction(const Request& r) {
    const auto rq = encode_nng(r);
    const auto rs = exch(rq);
    return decode_nng(rs);
}

Notification
Session::Impl::notification(void) {
    buffer nntf = notification_socket.recv();
    vector<unsigned char> ntf(nntf.data<unsigned char>(), nntf.data<unsigned char>() + nntf.size());
    return decode_nng_ntf(ntf);
}

Session::Session(void) :
    pimpl(make_unique<Session::Impl>()) {
}

Session::~Session(void) = default;

Response
Session::interaction(const Request& r) {
    return pimpl->interaction(r);
}

Notification
Session::notification(void) {
    return pimpl->notification();
}
