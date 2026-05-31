#include "ttd_keeper.hpp"
#include "papi_nexui_nngpp_session.hpp"
#include "papi_nexui_dummy_session.hpp"
#include "papi_logic.hpp"
#include "papi_interface.hpp"
#include <memory>

using namespace papi;
using namespace std;

static unique_ptr<ProprietaryApi> s_papi;

extern "C" enum PapiResult
papi_Proprietary_Startup_Sequence(void) noexcept {
    s_papi = make_unique<ProprietaryLogic>(move(make_unique<dummy::NexuiSession>()), TtdKeeper::instance());
    return s_papi->Proprietary_Startup_Sequence();
}

extern "C" enum ProcedureResult
papi_Diagnostics_Maintenance_Recovery(void) noexcept {
    return s_papi->Diagnostics_Maintenance_Recovery();
}

extern "C" enum PapiResult
papi_Specific_Processing_Based_On_Pan(void) noexcept {
    return s_papi->Specific_Processing_Based_On_Pan();
}

extern "C" enum PapiCvmResult
papi_Proprietary_Cvm_Condition_Code_Processing(const struct CvRule cvRule) noexcept {
    return s_papi->Proprietary_Cvm_Condition_Code_Processing(cvRule);
}

extern "C" enum PapiCvmResult
papi_Proprietary_Cvm_Support_Check(const struct CvRule cvRule) noexcept {
    return PAPI_CVM_NOT_SUPPORTED;
}

extern "C" enum PapiResult
papi_Specific_Processing_Based_On_Fci() noexcept {
    return PAPI_OK;
}
