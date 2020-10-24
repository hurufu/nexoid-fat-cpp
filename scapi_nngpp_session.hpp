#pragma once

#include "scapi_session.hpp"
#include <nngpp/nngpp.h>

namespace scapi::nngpp {

    class Session : public ::scapi::Session {
    public:
        Session(void);
        ~Session(void) noexcept override = default;

        ::std::unique_ptr<::scapi::Response>
        interaction(const ::scapi::Request&) override;

        ::std::unique_ptr<::scapi::Notification>
        notification(void) override;
    };

}
