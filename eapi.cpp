extern "C" {
#include "nexoid-ed/include/eapi.h"
#include "nexoid-ed/include/gtd.h"
}
#include "ttd_keeper.hpp"
#include "utils.hpp"
#include <cstring>

using namespace std;

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

struct CvmList s_cvmListEncipheredPin = [](){
    struct CvmList ret{};
    ret.amountX = 0;
    ret.amountY = 0;
    ret.cvRule.l = 3;
    ret.cvRule.a[0].cvmCode.cvm = CVM_SIGNATURE;
    ret.cvRule.a[0].cvmCode.applyRuleOnFail = 1;
    ret.cvRule.a[0].cvmConditionCode = CVM_RESERVED_01;
    ret.cvRule.a[1].cvmCode.cvm = CVM_ENCIPHERED_PIN_VERIFIED_BY_ICC_AND_SIGNATURE;
    ret.cvRule.a[1].cvmCode.applyRuleOnFail = 1;
    ret.cvRule.a[1].cvmConditionCode = CVM_IF_ALWAYS;
    ret.cvRule.a[2].cvmCode.cvm = CVM_SIGNATURE;
    ret.cvRule.a[2].cvmCode.applyRuleOnFail = 0;
    ret.cvRule.a[2].cvmConditionCode = CVM_IF_ALWAYS;
    return ret;
}();

enum EapiResult
eapi_Activate_Contacts_And_Reset_Chip(void) {
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
eapi_Reset_Chip(void) {
    return EAPI_NOK;
}

enum EapiResult
eapi_Select_Pse(void) {
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
eapi_Select_Application(const struct Aid* const aid) {
    //char buf[aid->l_raw * 2 + 1];
    //for (size_t i = 0; i < aid->l_raw; i++) {
    //    if (4 == i) {
    //        buf[i*2] = ' ';
    //    }
    //    sprintf(buf + 2 * i, "%02X", aid->raw[i]);
    //}
    //MSG(N,F,R,C, "AID [%s]\n", buf);
    cr = acp((struct CardResponse){
        .responseData = nullptr,
        .parsed = nullptr,
        .sw1Sw2 = { .e = I_COMMAND_OK }
    });
    return EAPI_OK;
}

enum EapiResult
eapi_Read_Record(const uint8_t p1, const uint8_t p2) {
    //MSG(N,F,R,C, "P1P2 [ %02"PRIX8" %02"PRIX8" ]\n", p1, p2);
    //MSG(N,F,L,C, "(no response)\n");
    cr = acp((struct CardResponse){
        .responseData = nullptr,
        .parsed = nullptr,
        .sw1Sw2 = { .e = I_COMMAND_OK }
    });
    return EAPI_OK;
}

enum EapiResult
eapi_Final_Application_Selection(void) {
    //char buf[e1kd->wd.aidTerminal.l_raw * 2 + 1];
    //for (size_t i = 0; i < e1kd->wd.aidTerminal.l_raw; i++) {
    //    if (4 == i) {
    //        buf[i*2] = ' ';
    //    }
    //    sprintf(buf + 2 * i, "%02X", e1kd->wd.aidTerminal.raw[i]);
    //}
    //MSG(N,F,R,C, "AID [%s]\n", buf);
    //char buf2[s_Fci.a5.pdol->s * 4 + 10];
    //MSG(N,F,L,C, "PDOL %s\n", bin_tostring(s_Fci.a5.pdol->s, s_Fci.a5.pdol->a, &buf2));
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
eapi_Select_Next_Application(const struct Aid* const aid) {
    //char buf[aid->l_raw * 3 + 1];
    //for (size_t i = 0, j = 0; i < aid->l_raw; i++, j+=2) {
    //    if (4 == i) {
    //        buf[j++] = ' ';
    //    }
    //    sprintf(buf + j, "%02X", aid->raw[i]);
    //}
    //MSG(N,F,R,C, "AID [%s]\n", buf);

    return EAPI_OK;
}

enum EapiResult
eapi_Get_Processing_Options(const uint8_t commandTemplate,
                            const struct DolData* const cdaTransactionData) {
    //char bufDol[cdaTransactionData->s * 4 + 10];
    //MSG(N,F,R,C, "Command Template [ %02X %02X ], DOL %s\n", (int)commandTemplate,(int)cdaTransactionData->s, bin_tostring(cdaTransactionData->s, cdaTransactionData->c, &bufDol));
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
    //char bufAip[sizeof(s_gpoResponse.aip.a) * 4 + 10];
    //char bufAfl[sizeof(s_gpoResponse.afl.raw_size) * 4 + 10];
    //MSG(N,F,L,C, "AID %s, AFL %s\n", bin_tostring(sizeof(s_gpoResponse.aip.a), s_gpoResponse.aip.a, &bufAip), bin_tostring(s_gpoResponse.afl.raw_size, s_gpoResponse.afl.raw, &bufAfl));
    return EAPI_OK;
}

enum EapiResult
eapi_External_Authenticate(void) {
    return EAPI_OK;
}

static enum EapiResult verify_enciphered_pin_block(const uint8_t p2) {
    //char buf[8*4+10];
    //MSG(N,F,R,C, "Enciphered PIN Block %s P2: [%02X]\n", bin_tostring(e1kd->cd.pinData.encipheredPinData->l, e1kd->cd.pinData.encipheredPinData->r, &buf), p2);
    cr = acp((struct CardResponse){
        .responseData = nullptr,
        .parsed = nullptr,
        .sw1Sw2 = { .e = I_COMMAND_OK }
    });
    //MSG(X,F,L,C, "SW1SW2 %s [%02X %02X]\n", EmvStatus_tostring(cr->sw1Sw2), cr->sw1Sw2.raw[0], cr->sw1Sw2.raw[1]);
    return EAPI_OK;
}

enum EapiResult
eapi_Verify_Pin_Block(const uint8_t p2) {
    static const uint8_t s_expected_pin_block[8] = {
        0x25, 0x11, 0x22, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF
    };
    static uint8_t s_pin_try_counter = 3;

    if (!e1kd->cd.pinData.offlinePinBlock) {
        if (!e1kd->cd.pinData.encipheredPinData) {
            //MSG(E,F,R,C, "No PIN block\n");
            return EAPI_NOK;
        }
        return verify_enciphered_pin_block(p2);
       }

    //char buf[8*4+10];
    //MSG(N,F,R,C, "PIN Block %s P2: [%02X]\n", bin_tostring(8, e1kd->cd.pinData.offlinePinBlock->raw, &buf), p2);

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
    //MSG(N,F,L,C, "SW1SW2 %s [%02X %02X]\n", EmvStatus_tostring(cr->sw1Sw2), cr->sw1Sw2.raw[0], cr->sw1Sw2.raw[1]);
    return res;
}

enum EapiResult
eapi_Get_Challenge(void) {
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

    //char buf[s_unSize * 4 + 10];
    //MSG(N,F,L,C, "Unpredictable number [ %s ]\n", bin_tohex(s_unSize, cr->parsed->unpredictableNumber->c, &buf));
    return EAPI_OK;
}
