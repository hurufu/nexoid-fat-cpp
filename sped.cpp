extern "C" {
#include <nexoid/scapi.h>
}

enum ScapiResult
sped_Generate_Enciphered_Pin_Data(struct CvRule cvRule) {
    return SCAPI_NOK;
}

enum ScapiResult
sped_Generate_Offline_Pin_Block(struct CvRule cvRule) {
    return SCAPI_NOK;
}

enum ScapiResult
sped_Encrypt_Pin_Block(const struct EncipherablePinBlock* const pb, struct binary** const epb) {
    return SCAPI_NOK;
}
