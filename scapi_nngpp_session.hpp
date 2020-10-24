#pragma once

#include "scapi_session.hpp"
#include <experimental/propagate_const>

namespace scapi::nngpp {

    class Session : public ::scapi::Session {
        struct Impl;
        ::std::experimental::propagate_const<::std::unique_ptr<Impl>> pimpl;
    public:
        Session(void);
        ~Session(void) noexcept override;

        ::std::unique_ptr<::scapi::Response>
        interaction(const ::scapi::Request&) override;

        ::std::unique_ptr<::scapi::Notification>
        notification(void) override;
    };

}
