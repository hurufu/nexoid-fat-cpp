#pragma once

#include "scapi_session.hpp"

class ScapiSocketSession : public ScapiSession {
public:
    ScapiSocketSession(void);
    ~ScapiSocketSession(void) noexcept override;
    void* interaction(void*) const override;
    void* notification(void) const override;
};
