#pragma once

#include "scapi_session.hpp"

#include <libsocket/inetclientstream.hpp>

namespace scapi::socket {

    class Session : public ::scapi::Session {
        libsocket::inet_stream stream;
    public:
        Session(void);
        ~Session(void) noexcept override;

        ::std::unique_ptr<::scapi::Response>
        interaction(const ::scapi::Request&) const override;

        ::std::unique_ptr<::scapi::Notification>
        notification(void) const override;
 
    };

}
