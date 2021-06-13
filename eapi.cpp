extern "C" {
#include "nexoid-ed/include/eapi.h"
#include "nexoid-ed/include/gtd.h"
}
#include "ttd_keeper.hpp"
#include "utils.hpp"
#include "eapi_session.hpp"
#include "eapi_dummy_session.hpp"
#include <cstring>

using namespace std;

static unique_ptr<EapiSession> s_e = make_unique<EapiDummy>();

enum EapiResult
eapi_Activate_Contacts_And_Reset_Chip(void) {
    return s_e->Activate_Contacts_And_Reset_Chip();
}

enum EapiResult
eapi_Reset_Chip(void) {
    return s_e->Reset_Chip();
}

enum EapiResult
eapi_Select_Pse(void) {
    return s_e->Select_Pse();
}

enum EapiResult
eapi_Select_Application(const struct Aid* const aid) {
    return s_e->Select_Application(*aid);
}

enum EapiResult
eapi_Read_Record(const uint8_t p1, const uint8_t p2) {
    return s_e->Read_Record(byte(p1), byte(p2));
}

enum EapiResult
eapi_Final_Application_Selection(void) {
    return s_e->Final_Application_Selection();
}

enum EapiResult
eapi_Select_Next_Application(const struct Aid* const aid) {
    return s_e->Select_Next_Application(*aid);
}

enum EapiResult
eapi_Get_Processing_Options(const uint8_t commandTemplate,
                            const struct DolData* const cdaTransactionData) {
    return s_e->Get_Processing_Options(byte(commandTemplate), *cdaTransactionData);
}

enum EapiResult
eapi_External_Authenticate(void) {
    return s_e->External_Authenticate();
}

enum EapiResult
eapi_Verify_Pin_Block(const uint8_t p2) {
    return s_e->Verify_Pin_Block(byte(p2));
}

enum EapiResult
eapi_Get_Challenge(void) {
    return s_e->Get_Challenge();
}

enum EapiResult
eapi_Generate_Ac(void) {
    return s_e->Generate_Ac();
}
