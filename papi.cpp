extern "C" {
#include <nexoid/papi.h>
#include <nexoid/gtd.h>
}

enum PapiResult
papi_Proprietary_Startup_Sequence(void) {
    return PAPI_NOK;
}

enum ProcedureResult
papi_Diagnostics_Maintenance_Recovery(void) {
    return PR_NOK;
}

void
papi_Force_Reboot(void) {
}

void
papi_Force_Termination(void) {
}

enum PapiResult
papi_Specific_Processing_Based_On_Pan(void) {
    return PAPI_NOK;
}

enum PapiCvmResult
papi_Proprietary_Cvm_Condition_Code_Processing(const struct CvRule cvRule) {
    return PAPI_CVM_NOT_APPLICABLE;
}

enum PapiCvmResult
papi_Proprietary_Cvm_Support_Check(const struct CvRule cvRule) {
    return PAPI_CVM_NOT_SUPPORTED;
}
