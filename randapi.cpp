extern "C" {
#include "nexoid-ed/include/scapi.h"
#include "nexoid-ed/include/gtd.h"
}

#include "exceptions.hpp"
#include <random>
#include <algorithm>
#include <functional>

using namespace std;

random_device s_random_device;
auto s_random_byte_generator = bind(uniform_int_distribution<uint8_t>(), ref(s_random_device));

enum ScapiResult
randapi_Generate_Random_Number(const uint8_t upperLimit, uint8_t* const randomNumber) noexcept try {
    if (!randomNumber) {
        throw null_argument({randomNumber}, "Nowhere to put generated value");
    }
    uniform_int_distribution<uint8_t> dist(0, upperLimit);
    *randomNumber = dist(s_random_device);
    return SCAPI_OK;
} catch (...) {
    ttd.terminalErrorReason = TE_HARDWARE_ERROR;
    ttd.terminalErrorIndicator = true;
    return SCAPI_NOK;
}

enum ScapiResult
randapi_Generate_Random_Bytes(const size_t size, uint8_t randomBytes[]) noexcept try {
    if (!randomBytes) {
        throw null_argument({randomBytes}, "Nowhere to put generated values");
    }
    generate(randomBytes, randomBytes + size, ref(s_random_byte_generator));
    return SCAPI_OK;
} catch (...) {
    ttd.terminalErrorReason = TE_HARDWARE_ERROR;
    ttd.terminalErrorIndicator = true;
    return SCAPI_NOK;
}
