#pragma once

#include "scapi_messages.hpp"

namespace scapi {

    class Session {
    public:
        virtual ~Session(void) noexcept = default;

        virtual ::std::unique_ptr<::scapi::Response>
        interaction(const ::scapi::Request&) const = 0;

        virtual ::std::unique_ptr<::scapi::Notification>
        notification(void) const = 0;
    };

}