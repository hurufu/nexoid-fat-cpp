#include "scapi_socket_session.hpp"

ScapiSocketSession::ScapiSocketSession(void) {
}

ScapiSocketSession::~ScapiSocketSession(void) noexcept {
}

void* ScapiSocketSession::interaction(void*) const {
    return nullptr;
}

void* ScapiSocketSession::notification(void) const {
    return nullptr;
}
