#pragma once

extern "C" {
#include "nexoid-ed/include/dmapi.h"
}

#include <limits>
#include <string>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <ctime>
#include <stdexcept>

template <typename L, typename R>
inline constexpr L
integer_cast(const R r) {
    if (r < 0) {
        if (::std::numeric_limits<L>::is_signed) {
            if (::std::numeric_limits<L>::min() > static_cast<intmax_t>(r)) {
                throw ::std::range_error("Integer conversion undeflow");
            }
        } else {
            throw ::std::range_error("Integer conversion failed. Can't fit negative number to unsigned type");
        }
    } else {
        if (r > ::std::numeric_limits<L>::max()) {
            throw ::std::range_error("Integer conversion overflow");
        }
    }
    return static_cast<L>(r);
}

template <typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
    return static_cast<typename std::underlying_type<E>::type>(e);
}

template <typename Container>
static auto
join(const Container& c, const std::string& delimeter = ", ") {
    auto it = c.begin();
    std::string ret = *it;
    it++;
    for (; it != c.end(); it++) {
        ret += delimeter + *it;
    }
    return ret;
}

template<typename Clock, typename Duration>
std::ostream&
operator << (std::ostream& os, const std::chrono::time_point<Clock, Duration>& tp) {
    const time_t t = Clock::to_time_t(tp);
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
    struct tm tm = {};
    return os << std::put_time(localtime_r(&t, &tm), "%F %T");
}

template <typename UnsignedInteger, typename Bcd>
Bcd convert_int_to_bcd(const UnsignedInteger from) {
    Bcd ret = { };
    UnsignedInteger tmp = from;
    for (ssize_t j = sizeof(ret.v) - 1; tmp && j >= 0; tmp /= 100, j--) {
        ret.v[j] = (uint8_t)((((tmp % 100) / 10) << 4) | (tmp % 10));
    }
    if (tmp != 0) {
        throw ::std::runtime_error("Integer conversion failed it didn't fit into requested BCD type");
    }
    return ret;
}

template <typename UnsignedInteger, typename Bcd>
UnsignedInteger convert_bcd_to_int(const Bcd from) {
    UnsignedInteger ret = 0, power = 1;
    for (size_t j = sizeof(from.v)/sizeof(from.v[0]) - 1; j != 0; j--, power *= 100) {
        ret += (from.v[j] & 0x0F) * power;
        ret += ((from.v[j] & 0xF0) >> 4) * power * 10;
        // TODO: Detect overflow
    }
    return ret;
}

template <typename T>
T* acp(const T& prototype) {
    auto ret = reinterpret_cast<T*>(dmapi_malloc(sizeof(T)));
    *ret = prototype;
    return ret;
}
