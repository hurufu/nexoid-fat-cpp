#pragma once

#include "eapi_session.hpp"

class EapiDummy : public EapiSession {
public:
    EapiDummy();
    ~EapiDummy() noexcept override;
    enum EapiResult Activate_Contacts_And_Reset_Chip() override;
    enum EapiResult Reset_Chip() override;
    enum EapiResult Select_Pse() override;
    enum EapiResult Select_Application(const struct Aid&) override;
    enum EapiResult Select_Next_Application(const struct Aid&) override;
    enum EapiResult Generate_Ac() override;
    enum EapiResult Read_Record(std::byte p1, std::byte p2) override;
    enum EapiResult Verify_Pin_Block(std::byte p2) override;
    enum EapiResult Get_Challenge() override;
    enum EapiResult Final_Application_Selection() override;
    enum EapiResult Get_Processing_Options(std::byte commandTemplate, const struct DolData& cdaTransactionData) override;
    enum EapiResult External_Authenticate() override;
};
