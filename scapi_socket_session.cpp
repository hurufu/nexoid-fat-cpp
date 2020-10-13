#include "scapi_socket_session.hpp"
#include "scapi_messages_asn1c.hpp"
#include "utils.hpp"

#include <vector>
#include <stdexcept>

using namespace ::std;
using namespace ::scapi::socket;

vector<unsigned char>
Session::exch(const vector<unsigned char> req) {
    if (stream.snd(req.data(), req.size(), 0) != integer_cast<ssize_t>(req.size())) {
        throw runtime_error("Couldn't send full message");
    }
    vector<unsigned char> buf(2 * 1024);
    const ssize_t received = stream.rcv(buf.data(), buf.capacity(), 0);
    if (received < 0) {
        throw runtime_error("No message");
    }
    buf.resize(received);
    return buf;
}

Session::Session(void)
    : stream("localhost", "50153", LIBSOCKET_BOTH) {
    try {
        decode(exch(encode(RegistrationRequest{})));
    } catch (...) {
        stream.shutdown(LIBSOCKET_READ | LIBSOCKET_WRITE);
        stream.destroy();
        throw;
    }
}

Session::~Session(void) noexcept {
    stream.shutdown(LIBSOCKET_READ | LIBSOCKET_WRITE);
    stream.destroy();
}

unique_ptr<::scapi::Response>
Session::interaction(const ::scapi::Request& r) {
    return make_unique<::scapi::Response>(get<0>(decode(exch(encode(Request(r))))));
}

unique_ptr<::scapi::Notification>
Session::notification(void) {
    return make_unique<::scapi::Notification>(get<1>(decode(exch(encode(NotificationRequest{})))));
}
