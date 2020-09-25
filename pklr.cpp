extern "C" {
#include <nexoid/pklr.h>
}

enum PklrResult
pklr_Process_Read_Record(const uint8_t p1, const uint8_t p2) {
    return PKLR_NOK;
}

enum PklrResult
pklr_Offline_Data_Authentication(void) {
    return PKLR_NOK;
}

enum PklrResult
pklr_Build_Candidate_List(void) {
    return PKLR_NOK;
}

enum PklrResult
pklr_Kernel_E_Processing(void) {
    return PKLR_NOK;
}

enum PklrResult
pklr_First_Generate_Ac_Processing(const uint8_t p1) {
    return PKLR_NOK;
}

enum PklrResult
pklr_Second_Generate_Ac_Processing(const uint8_t p1) {
    return PKLR_NOK;
}

enum PklrResult
pklr_First_Issuer_Script_Processing(void) {
    return PKLR_NOK;
}

enum PklrResult
pklr_Second_Issuer_Script_Processing(void) {
    return PKLR_NOK;
}
