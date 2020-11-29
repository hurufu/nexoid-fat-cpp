#pragma once

#include "scapi_messages.hpp"

#include <chrono>

namespace scapi {

    class Session {
    public:
        virtual ~Session(void) noexcept = default;

        virtual ::scapi::Response
        interaction(const ::scapi::Request&, std::chrono::milliseconds = std::chrono::milliseconds(-1)) = 0;

        virtual ::scapi::Notification
        notification(void) = 0;
    };

}
