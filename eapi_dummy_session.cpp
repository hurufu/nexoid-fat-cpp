extern "C" {
#include "nexoid-ed/include/gtd.h"
#include "nexoid-ed/include/eapi.h"
}
#include "eapi_dummy_session.hpp"
#include "utils.hpp"
#include "ttd_keeper.hpp"
#include <cstring>

using namespace std;

EapiDummy::EapiDummy() = default;

EapiDummy::~EapiDummy() noexcept = default;

enum EapiResult
EapiDummy::Activate_Contacts_And_Reset_Chip() {
    const int test_variant = 0;
    switch (test_variant) {
    case 0:
        TtdKeeper::instance().update(TECH_EMV_CHIP);
        return EAPI_OK;
    case 1:
        TtdKeeper::instance().update(TECH_NON_EMV_CHIP);
        return EAPI_OK;
    case 2:
        TtdKeeper::instance().update(N_CARD_MISSING);
        return EAPI_NOK;
    case 3:
        TtdKeeper::instance().update(N_CHIP_ERROR);
        return EAPI_NOK;
    }
    return EAPI_NOK;
}

enum EapiResult
EapiDummy::Reset_Chip() {
    return EAPI_NOK;
}

enum EapiResult
EapiDummy::Select_Pse() {
    return EAPI_NOK;
}

enum EapiResult
EapiDummy::Select_Application(const struct Aid& aid) {
    cr = acp((struct CardResponse){
        .responseData = nullptr,
        .parsed = nullptr,
        .sw1Sw2 = { .e = I_COMMAND_OK }
    });
    return EAPI_OK;
}

enum EapiResult
EapiDummy::Read_Record(const byte p1, const byte p2) {
    cr = acp((struct CardResponse){
        .responseData = nullptr,
        .parsed = nullptr,
        .sw1Sw2 = { .e = I_COMMAND_OK }
    });
    return EAPI_OK;
}

enum EapiResult
EapiDummy::Final_Application_Selection() {
    return EAPI_NOK;
}

enum EapiResult
EapiDummy::Select_Next_Application(const struct Aid& aid) {
    return EAPI_OK;
}

enum EapiResult
EapiDummy::Get_Processing_Options(const byte commandTemplate, const struct DolData& cdaTransactionData) {
    struct ResponseMessageTemplate gpoResponse = [](){
        struct ResponseMessageTemplate ret{};
        ret.aip.cardholderVerificationIsSupported = 1;
        ret.aip.sdaSupported = 1;
        ret.afl.raw_size = 4;
        ret.afl.a[0].sfi = 5;
        ret.afl.a[0].firstRecordNumber = 1;
        ret.afl.a[0].lastRecordNumber = 1;
        ret.afl.a[0].numberOfOdaAuthenticatedRecords = 0;
        return ret;
    }();
    cr = acp((struct CardResponse){
        .responseData = nullptr,
        .parsed = acp((struct ParsedResponseData){
            .fci = nullptr,
            .responseMessageTemplate = &gpoResponse,
            .readRecordResponeMessageTemplate = nullptr,
            .unpredictableNumber = nullptr,
        }),
        .sw1Sw2 = { .e = I_COMMAND_OK }
    });
    return EAPI_OK;
}

enum EapiResult
EapiDummy::External_Authenticate(void) {
    return EAPI_OK;
}

enum EapiResult
EapiDummy::Verify_Pin_Block(const byte p2) {
    static const uint8_t s_expected_pin_block[8] = {
        0x25, 0x11, 0x22, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF
    };
    static uint8_t s_pin_try_counter = 3;


    if (!e1kd->cd.pinData.offlinePinBlock) {
        if (!e1kd->cd.pinData.encipheredPinData) {
            return EAPI_NOK;
        }
        cr = acp((struct CardResponse){
            .responseData = nullptr,
            .parsed = nullptr,
            .sw1Sw2 = { .e = I_COMMAND_OK }
        });
        return EAPI_OK;
    }

    enum EapiResult res = EAPI_NOK;
    if (0 == s_pin_try_counter) {
        cr->sw1Sw2.e = E_AUTHENTICATION_METHOD_BLOCKED;
        res = EAPI_OK;
    } else {
        cr = acp((struct CardResponse){});
        if (memcmp(e1kd->cd.pinData.offlinePinBlock->raw, s_expected_pin_block, 8) == 0) {
            s_pin_try_counter = 3;
            cr->sw1Sw2.e = I_COMMAND_OK;
            res = EAPI_OK;
        } else {
            s_pin_try_counter--;
            const union EmvStatus tmp = { .raw = { 0x63, integer_cast<uint8_t>(0xC0 + s_pin_try_counter) } };
            cr->sw1Sw2 = tmp;
            res = EAPI_OK;
        }
    }
    return res;
}

enum EapiResult
EapiDummy::Get_Challenge() {
    cr = acp((struct CardResponse){
        .responseData = nullptr,
        .parsed = acp((struct ParsedResponseData){
            .fci = nullptr,
            .responseMessageTemplate = nullptr,
            .readRecordResponeMessageTemplate = nullptr,
            .unpredictableNumber = acp((struct UnpredictableNumber){})
        }),
        .sw1Sw2 = { .e = I_COMMAND_OK }
    });
    static const size_t s_unSize = sizeof(cr->parsed->unpredictableNumber->c);
    for (size_t i = 0; i < s_unSize; i++) {
        cr->parsed->unpredictableNumber->c[i] = integer_cast<uint8_t>(rand() % numeric_limits<uint8_t>::max());
    }
    return EAPI_OK;
}

enum EapiResult
EapiDummy::Generate_Ac() {
    return EAPI_OK;
}
