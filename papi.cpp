#include "scapi_internal.hpp"
#include "tostring.hpp"
#include "ttd_keeper.hpp"

extern "C" {
#include <nexoid/papi.h>
}

#include <iostream>

using namespace std;

extern "C" enum PapiResult
papi_Proprietary_Startup_Sequence(void) noexcept try {
    if (scapi_Initialize() != SCAPI_OK) {
        cout << "SCAPI Initialization failed" << endl;
        return PAPI_NOK;
    }
    return PAPI_OK;
} catch (...) {
    TtdKeeper::instance().handle_exception(__func__);
    return PAPI_NOK;
}

extern "C" enum ProcedureResult
papi_Diagnostics_Maintenance_Recovery(void) noexcept try {
    cout << __FILE__ << ':' << __LINE__ << '@' << __PRETTY_FUNCTION__
         << ' ' << TtdKeeper::instance().fetch_ter_reason()
         << ' ' << TtdKeeper::instance().fetch_nok_reason()
         << endl;
    return PR_NOK;
} catch (...) {
    TtdKeeper::instance().handle_exception();
    return PR_NOK;
}

extern "C" enum PapiResult
papi_Specific_Processing_Based_On_Pan(void) noexcept try {
    return PAPI_NOK;
} catch (...) {
    TtdKeeper::instance().handle_exception(__func__);
    return PAPI_NOK;
}

extern "C" enum PapiCvmResult
papi_Proprietary_Cvm_Condition_Code_Processing(const struct CvRule cvRule) noexcept try {
    return PAPI_CVM_NOT_APPLICABLE;
} catch (...) {
    TtdKeeper::instance().handle_exception(__func__);
    return PAPI_CVM_NOT_APPLICABLE;
}

extern "C" enum PapiCvmResult
papi_Proprietary_Cvm_Support_Check(const struct CvRule cvRule) noexcept try {
    return PAPI_CVM_NOT_SUPPORTED;
} catch (...) {
    TtdKeeper::instance().handle_exception(__func__);
    return PAPI_CVM_NOT_SUPPORTED;
}
