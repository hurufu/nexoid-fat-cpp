#include "scapi_internal.hpp"
#include "tostring.hpp"
#include "ttd_keeper.hpp"
#include "papi_nexui_nngpp_session.hpp"

extern "C" {
#include <nexoid/papi.h>
}

#include <iostream>
#include <memory>

using namespace papi;
using namespace std;

static unique_ptr<NexuiSession> s_nexui;

static NexuiRequest
create_maintenance_info(const enum TerminalErrorReason ter, const enum NokReason nok) {
    return {
        NexuiRequest::Api::output, {
            "Diagnostics",
            string("Terminal Error: ") + tostring(ter),
            string("Nok: ") + tostring(nok)
        }
    };
}

extern "C" enum PapiResult
papi_Proprietary_Startup_Sequence(void) noexcept try {
    s_nexui = make_unique<nngpp::NexuiSession>();
    s_nexui->interaction({ NexuiRequest::Api::output, {"Startup"}});
    if (scapi_Initialize() != SCAPI_OK) {
        cout << "SCAPI Initialization failed" << endl;
        s_nexui->interaction({ NexuiRequest::Api::output, {"SCAPI Initialization failed"} });
        return PAPI_NOK;
    }
    return PAPI_OK;
} catch (...) {
    TtdKeeper::instance().handle_exception(__func__);
    return PAPI_NOK;
}

extern "C" enum ProcedureResult
papi_Diagnostics_Maintenance_Recovery(void) noexcept try {
    const auto ter = TtdKeeper::instance().fetch_ter_reason();
    const auto nok = TtdKeeper::instance().fetch_nok_reason();
    cout << __FILE__ << ':' << __LINE__ << '@' << __func__
         << ' ' << ter << ' ' << nok << endl;
    s_nexui->interaction(create_maintenance_info(ter, nok));
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
