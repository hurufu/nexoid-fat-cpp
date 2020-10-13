#include "scapi_socket_session.hpp"

using namespace ::scapi::socket;

Session::Session(void) {
}

Session::~Session(void) noexcept {
}


std::unique_ptr<::scapi::Response>
Session::interaction(const ::scapi::Request&) const {
    return std::make_unique<::scapi::Response>();
}

std::unique_ptr<::scapi::Notification>
Session::notification(void) const {
    return std::make_unique<::scapi::Notification>();
}
