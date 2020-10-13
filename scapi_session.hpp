#pragma once

class ScapiSession {
public:
    virtual ~ScapiSession(void) noexcept = default;
    virtual void* interaction(void*) const = 0;
    virtual void* notification(void) const = 0;
};
