#pragma once

template <typename Base>
class Singleton {
public:
    static Base& instance() {
        static Base s_base{};
        return s_base;
    }
    Singleton(const Singleton&) = delete;
    Singleton& operator= (const Singleton&) = delete;
protected:
    Singleton() = default;
};
