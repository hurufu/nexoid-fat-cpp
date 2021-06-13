extern "C" {
#include "nexoid-ed/include/scapi.h"
#include "nexoid-ed/include/gtd.h"
}

#include <random>

using namespace std;

enum ScapiResult
randapi_Generate_Random_Number(const uint8_t upperLimit, uint8_t* const randomNumber) noexcept try {
    random_device rd;
    uniform_int_distribution<uint8_t> dist(0, upperLimit);
    *randomNumber = dist(rd);
    return SCAPI_OK;
} catch (...) {
    ttd.terminalErrorReason = TE_HARDWARE_ERROR;
    ttd.terminalErrorIndicator = true;
    return SCAPI_NOK;
}

enum ScapiResult
randapi_Generate_Random_Bytes(const size_t size, uint8_t randomBytes[]) noexcept {
    return SCAPI_NOK;
}
