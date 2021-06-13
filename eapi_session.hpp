#pragma once

extern "C" {
#include "nexoid-ed/include/types.h"
}
#include <cstddef>

struct EapiSession {
    virtual ~EapiSession() noexcept = default;
    virtual EapiResult Activate_Contacts_And_Reset_Chip() = 0;
    virtual EapiResult Reset_Chip() = 0;
    virtual EapiResult Select_Pse() = 0;
    virtual EapiResult Select_Application(const struct Aid&) = 0;
    virtual EapiResult Select_Next_Application(const struct Aid&) = 0;
    virtual EapiResult Generate_Ac() = 0;
    virtual EapiResult Read_Record(std::byte p1, std::byte p2) = 0;
    virtual EapiResult Verify_Pin_Block(std::byte p2) = 0;
    virtual EapiResult Get_Challenge() = 0;
    virtual EapiResult Final_Application_Selection() = 0;
    virtual EapiResult Get_Processing_Options(std::byte commandTemplate, const struct DolData& cdaTransactionData) = 0;
    virtual EapiResult External_Authenticate() = 0;
};
