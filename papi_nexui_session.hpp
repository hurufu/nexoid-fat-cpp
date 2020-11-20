#pragma once

#include <list>
#include <string>

namespace papi {

    struct NexuiRequest {
        enum class Api {
            output,
            entry,
            ssn
        };

        Api api;
        std::list<std::string> lines;
    };
    struct NexuiResponse {
    };

    class NexuiSession {
    public:
        virtual ~NexuiSession(void) noexcept = default;

        virtual ::papi::NexuiResponse interaction(const ::papi::NexuiRequest&) = 0;
    };

}
