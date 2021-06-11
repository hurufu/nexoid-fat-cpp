extern "C" {
#include "nexoid-ed/include/pklr.h"
#include "nexoid-ed/include/gtd.h"
#include "nexoid-ed/src/NexoFast.h"
}

#include "utils.hpp"

// FIXME: Remote it!
extern struct CandidateList* g_CandidateList;

enum PklrResult
pklr_Process_Read_Record(const uint8_t p1, const uint8_t p2) {
    ttd.pan = acp((struct Prefix){
        16,
        "4761739001010119"
    });
    e1kd->cd.cvmList = acp([](){
        struct CvmList ret{};
        ret.amountX = 0;
        ret.amountY = 0;
        ret.cvRule.l = 3;
        ret.cvRule.a[0].cvmCode.cvm = CVM_SIGNATURE;
        ret.cvRule.a[0].cvmCode.applyRuleOnFail = 1;
        ret.cvRule.a[0].cvmConditionCode = CVM_RESERVED_01;
        ret.cvRule.a[1].cvmCode.cvm = CVM_PLAINTEXT_PIN_VERIFICATION_PERFORMED_BY_ICC;
        ret.cvRule.a[1].cvmCode.applyRuleOnFail = 1;
        ret.cvRule.a[1].cvmConditionCode = CVM_IF_ALWAYS;
        ret.cvRule.a[2].cvmCode.cvm = CVM_SIGNATURE;
        ret.cvRule.a[2].cvmCode.applyRuleOnFail = 0;
        ret.cvRule.a[2].cvmConditionCode = CVM_IF_ALWAYS;
        return ret;
    }());
    return PKLR_OK;
}

enum PklrResult
pklr_Offline_Data_Authentication(void) {
    return PKLR_SUCCESSFUL;
}

enum PklrResult
pklr_Build_Candidate_List(void) {
    static struct CandidateList cl = {
        .l_entry = 1,
        .entry = {}
    };
    cl.entry[0] = {};
    cl.entry[0].ApplicationPriorityIndicator = {};
    cl.entry[0].ApplicationLabel = { "TestApp", {} };
    cl.entry[0].ApplicationPreferredName = { "TestApp #1", {} };
    cl.entry[0].IssuerCodeTableIndex = ISO_CODE_TABLE_1;
    cl.entry[0].TerminalPriorityIndicator = 0;
    cl.entry[0].ApplicationPriorityIndicator.priority = 0;
    cl.entry[0].ApplicationPriorityIndicator.cardholderConfirmationRequired = 0;
    cl.entry[0].DfName = (struct Aid){
        .l_raw = 6,
        .raw = { 0xA0, 0x00, 0x00, 0x00, 0x00, 0x01 }
    };
    g_CandidateList = &cl;
    ttd.processingStatus.buildingCandidateListUsingListOfAid = 1;
    ep.cd.applicationEffectiveDate = acp((union yymmdd){ 0x15, 0x01, 0x01 });
    ep.cd.applicationExpirationDate = acp((union yymmdd){ 0x25, 0x12, 0x31 });
    return PKLR_OK;
}

enum PklrResult
pklr_Kernel_E_Processing(void) {
#   if 1
    return Kernel_E_Processing();
#   else
    ttd.outcome = O_ONLINE_REQUEST;
    return PKLR_DONE;
#   endif
}

enum PklrResult
pklr_First_Generate_Ac_Processing(const uint8_t p1) {
    ttd.onlineRequired = true;
    return PKLR_OK;
}

enum PklrResult
pklr_Second_Generate_Ac_Processing(const uint8_t p1) {
    ttd.tcReceived = true;
    ttd.onlineRequired = false;
    return PKLR_OK;
}

enum PklrResult
pklr_First_Issuer_Script_Processing(void) {
    return PKLR_OK;
}

enum PklrResult
pklr_Second_Issuer_Script_Processing(void) {
    return PKLR_OK;
}
