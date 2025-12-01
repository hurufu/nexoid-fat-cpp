#include "scapi_nngpp_session.hpp"
#include "scapi_messages_asn1c.hpp"
#include "utils.hpp"

#include <nngpp/nngpp.h>
#include <nngpp/protocol/req0.h>
#include <nngpp/protocol/pull0.h>

using namespace ::std;
using namespace ::std::chrono;
using namespace ::scapi::nngpp;
using namespace ::nng;
using ::scapi::Response;
using ::scapi::Request;
using ::scapi::Notification;

class RecvTimeoutGuard {
    ::socket& socket;
    nng_duration stored_timeout;
    bool ignore = true;
public:
    RecvTimeoutGuard(::socket& s, const nng_duration timeout)
            : socket(s)
            , stored_timeout(get_opt_recv_timeout(s)) {
        if (timeout >= 0) {
            set_opt_recv_timeout(socket, timeout);
            ignore = false;
        }
    }
    ~RecvTimeoutGuard(void) noexcept {
        if (!ignore) {
            set_opt_recv_timeout(socket, stored_timeout);
        }
    }
};

struct Session::Impl {
    ::socket interaction_socket;
    ::socket notification_socket;
    int req_counter = 0;

    Impl(void);
    ~Impl(void) = default;

    vector<unsigned char> exch(const vector<unsigned char>&);
    Response interaction(const Request& r, milliseconds);
    Notification notification(void);
};

Session::Impl::Impl(void) :
    interaction_socket(req::open()),
    notification_socket(pull::open()) {
    set_opt_recv_timeout(notification_socket, 1 * 60 * 60 * 1000);
    set_opt_recv_timeout(interaction_socket, 3 * 1000);
    set_opt_send_timeout(interaction_socket, 1 * 1000);
    interaction_socket.dial("tcp://cvend-11e553e2.local:5001"); // fatrq
    notification_socket.listen("tcp://0.0.0.0:5002"); // fatnt
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
Session::Impl::interaction(const Request& r, const milliseconds rcv_timeout) {
    const RecvTimeoutGuard guard(interaction_socket, integer_cast<nng_duration>(rcv_timeout.count()));
    const auto rq = encode_nng({r, ++req_counter});
    const auto rs = exch(rq);
    return decode_nng(rs).rsp;
}

Notification
Session::Impl::notification(void) {
    const RecvTimeoutGuard guard(notification_socket, 10 * 1000);
    buffer nntf = notification_socket.recv();
    vector<unsigned char> ntf(nntf.data<unsigned char>(), nntf.data<unsigned char>() + nntf.size());
    return decode_nng_ntf(ntf).ntf;
}

Session::Session(void) :
    pimpl(make_unique<Session::Impl>()) {
}

Session::~Session(void) noexcept = default;

Response
Session::interaction(const Request& r, const milliseconds rcv_timeout) {
    return pimpl->interaction(r, rcv_timeout);
}

Notification
Session::notification(void) {
    return pimpl->notification();
}
