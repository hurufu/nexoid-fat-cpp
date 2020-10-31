#include "scapi_internal.hpp"
#include "tostring.hpp"
#include "ttd_keeper.hpp"

extern "C" {
#include <nexoid/papi.h>
}

#include <stdexcept>
#include <iostream>

using namespace std;

static PapiResult
papi_handle_exception(void) noexcept try {
    throw;
} catch (const exception& e) {
    cerr << __FILE__ << ':' << __LINE__ << '@' << __func__ << " Generic exception suppressed: " << e.what() << endl;
    return PAPI_NOK;
} catch (...) {
    cerr << __FILE__ << ':' << __LINE__ << '@' << __func__ << " Unexpected exception suppressed" << endl;
    return PAPI_NOK;
}

extern "C" enum PapiResult
papi_Proprietary_Startup_Sequence(void) noexcept try {
    if (scapi_Initialize() != SCAPI_OK) {
        cout << "SCAPI Initialization failed" << endl;
        return PAPI_NOK;
    }
    return PAPI_OK;
} catch (...) {
    return papi_handle_exception();
}

extern "C" enum ProcedureResult
papi_Diagnostics_Maintenance_Recovery(void) noexcept try {
    cout << __FILE__ << ':' << __LINE__ << '@' << __PRETTY_FUNCTION__
         << ' ' << TtdKeeper::instance().fetch_nok_reason()
         << ' ' << TtdKeeper::instance().fetch_ter_reason()
         << endl;
    return PR_NOK;
} catch (...) {
    papi_handle_exception();
    return PR_NOK;
}

extern "C" enum PapiResult
papi_Specific_Processing_Based_On_Pan(void) {
    return PAPI_NOK;
}

extern "C" enum PapiCvmResult
papi_Proprietary_Cvm_Condition_Code_Processing(const struct CvRule cvRule) {
    return PAPI_CVM_NOT_APPLICABLE;
}

extern "C" enum PapiCvmResult
papi_Proprietary_Cvm_Support_Check(const struct CvRule cvRule) {
    return PAPI_CVM_NOT_SUPPORTED;
}
