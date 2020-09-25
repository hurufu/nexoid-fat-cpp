extern "C" {
#include <nexoid/scapi.h>
}

enum ScapiResult
randapi_Generate_Random_Number(const uint8_t upperLimit, uint8_t* randomNumber) {
    return SCAPI_NOK;
}

enum ScapiResult
randapi_Generate_Random_Bytes(const size_t size, uint8_t randomBytes[]) {
    return SCAPI_NOK;
}
