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
    const uint8_t response[] = {
        0x6F, 0x15, // FCI Template
            0x84, 0x0E, // DF Name
                '1','P','A','Y','.','S','Y','S','.','D','D','F','0','1',
            0xA5, 0x03, // FCI Proprietary Template
                0x88, 0x01, // SFI
                    0x01
    };

    cr = acp((struct CardResponse){
        .responseData = acp((struct ResponseData){
            .s = sizeof(response),
            .c = {}
        }),
        .parsed = acp((struct ParsedResponseData){
            .fci = acp((struct FileControlInformation){
                .DfName = {
                    .l_raw = 14,
                    .raw = {
                        '1','P','A','Y','.','S','Y','S','.','D','D','F','0','1'
                    }
                },
                .a5 = {
                    .applicationLabel = nullptr,
                    .applicationPriorityIndicator = nullptr,
                    .pdol = nullptr,
                    .languagePreference = nullptr,
                    .issuerCodeTableIndex = nullptr,
                    .applicationPreferredName = nullptr,
                    .issuerDiscretionaryData = nullptr,
                    .sfi = acp((unsigned char)0x01),
                },
            }),
            .responseMessageTemplate = nullptr,
            .readRecordResponeMessageTemplate = nullptr,
            .unpredictableNumber = nullptr,
        }),
        .sw1Sw2 = { .e = I_COMMAND_OK }
    });
    memcpy(cr->responseData->c, response, sizeof(response));
    return EAPI_OK;
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
    struct FileControlInformation s_Fci = {
        .DfName = {
            .l_raw = 6,
            .raw = { 0xA0, 0x00, 0x00, 0x00, 0x00, 0x01 }
        },
        .a5 = {
            .applicationLabel = acp((struct string16){ "TestApp", {} }),
            .applicationPriorityIndicator = nullptr,
            .pdol = acp((struct Dol){
                .s = 9,
                .a = {
                    0x9F, 0x1A, 0x02, // Terminal Country Code
                    0x9F, 0x33, 0x03, // Terminal Capabilities
                    0x9F, 0x40, 0x02  // Additional Terminal Capabilities (partial)
                }
            }),
            .languagePreference = acp((struct TerminalSupportedLanguageList){
                .l = 1,
                .a = { { .e = LANG_ENGLISH } }
            }),
            .issuerCodeTableIndex = nullptr,
            .applicationPreferredName = nullptr,
            .issuerDiscretionaryData = nullptr,
            .sfi = nullptr,
        }
    };
    cr = acp((struct CardResponse){
        .responseData = nullptr,
        .parsed = acp((struct ParsedResponseData){
            .fci = &s_Fci,
            .responseMessageTemplate = nullptr,
            .readRecordResponeMessageTemplate = nullptr,
            .unpredictableNumber = nullptr,
        }),
        .sw1Sw2 = { .e = I_COMMAND_OK }
    });
    return EAPI_OK;
}

enum EapiResult
EapiDummy::Select_Next_Application(const struct Aid& aid) {
    return EAPI_OK;
}

enum EapiResult
EapiDummy::Get_Processing_Options(const byte commandTemplate, const struct DolData& cdaTransactionData) {
    struct ResponseMessageTemplate s_gpoResponse = [](){
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
            .responseMessageTemplate = &s_gpoResponse,
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
