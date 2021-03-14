#pragma once

#include <limits>
#include <string>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <ctime>
#include <stdexcept>

template <typename L, typename R>
L integer_cast(R r) {
    // TODO: Throw an exception if r doesnt fit into L
    return static_cast<L>(r);
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
    return os << std::put_time(localtime_r(&t, &tm), "%F %T.") <<
        std::setfill('0') << std::setw(3) << (ms - t * 1000);
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
