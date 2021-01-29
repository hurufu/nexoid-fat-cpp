extern "C" {
#include "nexoid-ed/include/eapi.h"
}

enum EapiResult
eapi_Activate_Contacts_And_Reset_Chip(void) {
    return EAPI_NOK;
}

enum EapiResult
eapi_Reset_Chip(void) {
    return EAPI_NOK;
}

enum EapiResult
eapi_Select_Pse(void) {
    return EAPI_NOK;
}

enum EapiResult
eapi_Select_Application(const struct Aid* const aid) {
    return EAPI_NOK;
}

enum EapiResult
eapi_Read_Record(const uint8_t p1, const uint8_t p2) {
    return EAPI_NOK;
}

enum EapiResult
eapi_Final_Application_Selection(void) {
    return EAPI_NOK;
}

enum EapiResult
eapi_Select_Next_Application(const struct Aid* const aid) {
    return EAPI_NOK;
}

enum EapiResult
eapi_Get_Processing_Options(const uint8_t commandTemplate,
                            const struct DolData* const cdaTransactionData) {
    return EAPI_NOK;
}

enum EapiResult
eapi_External_Authenticate(void) {
    return EAPI_NOK;
}

enum EapiResult
eapi_Verify_Pin_Block(const uint8_t p2) {
    return EAPI_NOK;
}

enum EapiResult
eapi_Get_Challenge(void) {
    return EAPI_NOK;
}
