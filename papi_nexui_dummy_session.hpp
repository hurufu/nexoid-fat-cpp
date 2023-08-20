#pragma once

#include "papi_nexui_session.hpp"
#include <memory>
#include <experimental/propagate_const>

namespace papi::dummy {

    class NexuiSession : public ::papi::NexuiSession {
    public:
        NexuiSession(void) = default;
        ~NexuiSession(void) noexcept override = default;

        ::papi::NexuiResponse interaction(const ::papi::NexuiRequest&) override;
    };

}
