#pragma once

#include <limits>

template <typename L, typename R>
L integer_cast(R r) {
    // TODO: Throw an exception if r doesnt fit into L
    return static_cast<L>(r);
}
