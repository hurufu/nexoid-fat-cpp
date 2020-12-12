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
std::ostream &operator<<(std::ostream &stream,
  const std::chrono::time_point<Clock, Duration> &time_point) {
  const time_t time = Clock::to_time_t(time_point);
  struct tm tm;
  localtime_r(&time, &tm);
  // FIXME: Print time with milliseconds
  return stream << std::put_time(&tm, "%F %T.       ");
}
