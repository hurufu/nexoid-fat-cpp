#pragma once

#include <limits>
#include <string>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <ctime>

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
  return os << std::put_time(localtime_r(&t, &tm), "%F %T.") << (ms - t * 1000);
}
