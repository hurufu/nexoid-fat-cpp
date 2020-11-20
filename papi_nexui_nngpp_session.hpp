#pragma once

#include "papi_nexui_session.hpp"
#include <memory>
#include <experimental/propagate_const>

namespace papi::nngpp {

    class NexuiSession : public ::papi::NexuiSession {
        struct Impl;
        ::std::experimental::propagate_const<::std::unique_ptr<Impl>> pimpl;
    public:
        NexuiSession(void);
        ~NexuiSession(void) noexcept override;

        ::papi::NexuiResponse interaction(const ::papi::NexuiRequest&) override;
    };

}
