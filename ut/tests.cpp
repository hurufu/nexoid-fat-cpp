#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "utils.hpp"

extern "C" {
#include "nexoid-ed/include/scapi.h"
}

#include <cstring>

template <typename T, T Value> static constexpr T Constant = Value;

struct TerminalTransactionData ttd;

TEST_CASE("Integer cast", "[utils]") {
    CHECK(integer_cast<int>(0) == 0);
    CHECK(integer_cast<int>(Constant<size_t,5>) == 5);
    CHECK(integer_cast<bool>(Constant<intmax_t,1>) == 1);
    CHECK(integer_cast<bool>(Constant<uintmax_t,0>) == 0);
    CHECK_THROWS(integer_cast<uint8_t>(Constant<int,-1>));
    CHECK_THROWS(integer_cast<int8_t>(Constant<int16_t,-129>));
}

TEST_CASE("Gen random number handles incorrect arguments", "[randapi]") {
    ttd = (struct TerminalTransactionData){};
    const auto r1 = randapi_Generate_Random_Number(24, nullptr);
    CHECK(r1 == SCAPI_NOK);
    CHECK(ttd.terminalErrorIndicator);
    CHECK(ttd.terminalErrorReason != TE_NONE);
}

TEST_CASE("Gen random bytes handles incorrect arguments", "[randapi]") {
    ttd = (struct TerminalTransactionData){};
    const auto r2 = randapi_Generate_Random_Bytes(10, nullptr);
    CHECK(r2 == SCAPI_NOK);
    CHECK(ttd.terminalErrorIndicator);
    CHECK(ttd.terminalErrorReason != TE_NONE);
}

TEST_CASE("Single random number is bounded by it's limits", "[randapi]") {
    uint8_t x;
    for (uint8_t u = 0; u != 99; u++) {
    for (int i = 3; i != 0; i--) {
        const auto res = randapi_Generate_Random_Number(u, &x);
        REQUIRE(res == SCAPI_OK);
        CHECK(x >= 0);
        CHECK(x <= u);
    }}
}

TEST_CASE("Generated random array isn't equal to the input buffer", "[randapi]") {
    static const uint8_t zero[10] = {};
    uint8_t buf[sizeof(zero)] = {};
    const auto r = randapi_Generate_Random_Bytes(sizeof(buf), buf);
    REQUIRE(r == SCAPI_OK);
    CHECK(memcmp(zero, buf, sizeof(buf)) != 0);
}
