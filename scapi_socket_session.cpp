#include "scapi_socket_session.hpp"
#include "scapi_messages_asn1c.hpp"
#include "utils.hpp"

#include <libsocket/exception.hpp>
#include <vector>
#include <stdexcept>
#include <iostream>

using namespace ::std;
using namespace ::scapi::socket;

inet_stream::inet_stream(const char* const host, const char* port)
    : ::libsocket::inet_stream(host, port, LIBSOCKET_BOTH) {
}

inet_stream::~inet_stream(void) noexcept {
    try {
        shutdown(LIBSOCKET_READ | LIBSOCKET_WRITE);
    } catch (const ::libsocket::socket_exception& e) {
        cerr <<  __FILE__ << ':' << __LINE__ << '@' << __func__ << " Internal exception suppressed (errno: " << e.err << ")\n" << e.mesg << endl;
    }
    destroy();
}

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
    : stream("localhost", "50153") {
        decode(exch(encode(RegistrationRequest{})));
}

unique_ptr<::scapi::Response>
Session::interaction(const ::scapi::Request& r) {
    return make_unique<::scapi::Response>(get<0>(decode(exch(encode(Request(r))))));
}

unique_ptr<::scapi::Notification>
Session::notification(void) {
    return make_unique<::scapi::Notification>(get<1>(decode(exch(encode(NotificationRequest{})))));
}
