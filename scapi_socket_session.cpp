#include "scapi_socket_session.hpp"

using namespace ::scapi::socket;

Session::Session(void)
    : stream("localhost", "50153", LIBSOCKET_BOTH) {
}

Session::~Session(void) noexcept {
    stream.shutdown(LIBSOCKET_READ | LIBSOCKET_WRITE);
}

std::unique_ptr<::scapi::Response>
Session::interaction(const ::scapi::Request&) const {
    return std::make_unique<::scapi::Response>();
}

std::unique_ptr<::scapi::Notification>
Session::notification(void) const {
    return std::make_unique<::scapi::Notification>();
}
