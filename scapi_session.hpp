#pragma once

#include "scapi_messages.hpp"

namespace scapi {

    class Session {
    public:
        virtual ~Session(void) noexcept = default;

        virtual ::scapi::Response
        interaction(const ::scapi::Request&) = 0;

        virtual ::scapi::Notification
        notification(void) = 0;
    };

}
