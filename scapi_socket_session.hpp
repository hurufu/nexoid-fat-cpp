#pragma once

#include "scapi_session.hpp"

#include <libsocket/inetclientstream.hpp>
#include <vector>

namespace scapi::socket {

    class Session : public ::scapi::Session {
        ::libsocket::inet_stream stream;
        ::std::vector<unsigned char> exch(const ::std::vector<unsigned char>);
    public:
        Session(void);
        ~Session(void) noexcept override;

        ::std::unique_ptr<::scapi::Response>
        interaction(const ::scapi::Request&) override;

        ::std::unique_ptr<::scapi::Notification>
        notification(void) override;
 
    };

}
