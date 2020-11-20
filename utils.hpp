#pragma once

#include <limits>
#include <string>

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
