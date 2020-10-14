#pragma once

#include "scapi_session.hpp"

#include <libsocket/inetclientstream.hpp>
#include <vector>

namespace scapi::socket {

    class inet_stream : public ::libsocket::inet_stream {
    public:
        inet_stream(const char* host, const char* port);
        ~inet_stream(void) noexcept;
    };

    class Session : public ::scapi::Session {
        inet_stream stream;
        ::std::vector<unsigned char> exch(const ::std::vector<unsigned char>);
    public:
        Session(void);
        ~Session(void) noexcept override = default;

        ::std::unique_ptr<::scapi::Response>
        interaction(const ::scapi::Request&) override;

        ::std::unique_ptr<::scapi::Notification>
        notification(void) override;
 
    };

}
