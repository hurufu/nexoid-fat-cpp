#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "utils.hpp"

template <typename T, T S> static constexpr T C = S;

TEST_CASE("Integer cast", "[utils]") {
    CHECK(integer_cast<int>(0) == 0);
    CHECK(integer_cast<int>(C<size_t,5>) == 5);
    CHECK(integer_cast<bool>(C<intmax_t,1>) == 1);
    CHECK(integer_cast<bool>(C<uintmax_t,0>) == 0);
    CHECK_THROWS(integer_cast<uint8_t>(C<int,-1>));
    CHECK_THROWS(integer_cast<int8_t>(C<int16_t,-129>));
}
