#include "scapi_internal.hpp"
#include "tostring.hpp"
#include "ttd_keeper.hpp"
#include "papi_nexui_nngpp_session.hpp"
#include "exceptions.hpp"
#include "utils.hpp"

#include "nexoid-ed/include/papi.h"

#include <iostream>
#include <memory>

using namespace papi;
using namespace std;
using namespace chrono;

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

static enum ProcedureResult
decide_what_kind_of_maintenance_is_required(const enum TerminalErrorReason ter) {
    switch (ter) {
        case TE_CONFIGURATION_ERROR:
        case TE_CRYPTOGRAPHIC_KEYS_MISSING:
        case TER_NOT_IMPLEMENTED:
        case TER_TIMEOUT:
            return PR_STARTUP_SEQUENCE;
        case TE_LOG_LIMIT_EXCEEDED:
        case TE_COMMUNICATION_ERROR:
        case TE_UNSPECIFIED:
        case TE_NEXO_FAST_FAILURE:
        case TE_INTERACTION_ERROR:
        case TE_OVERSPEND:
        case TER_INTERFACE_CONTRACT_VIOLATION:
        case TER_INTERNAL_ERROR:
            return PR_SHUTDOWN;
        case TER_OS_ERROR:
        case TE_HARDWARE_ERROR:
        case TE_MEMORY_FAILURE:
            return PR_REBOOT;
        case TE_NONE:
        case TER_MAX:
            return PR_NOK;
    }
    throw bad_mapping(ter, "Unknown TerminalErrorReason");
}

static const char*
map_maintenance_type_tostring(const enum ProcedureResult r) {
    switch (r) {
        case PR_STARTUP_SEQUENCE: return "Application restart requested";
        case PR_SHUTDOWN: return "Device shutdown requested";
        case PR_REBOOT: return "Device reboot requested";
        case PR_NOK: return "Nothing can help";
        default:
            throw bad_mapping(r, "Unknown maintenance type");
    }
}

extern "C" enum PapiResult
papi_Proprietary_Startup_Sequence(void) noexcept try {
    s_nexui = make_unique<nngpp::NexuiSession>();
    s_nexui->interaction({ NexuiRequest::Api::output, {"Startup"}});
    if (scapi_Initialize() != SCAPI_OK) {
        cout << system_clock::now() << " C nexoid-fat    "
             << __func__ << ": SCAPI Initialization failed" << endl;
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
    if (!s_nexui) {
        cout << system_clock::now() << " C nexoid-fat    "
             << __func__ << ": PAPI isn't initialized" << endl;
        return PR_NOK;
    }
    const auto ter = TtdKeeper::instance().update(TE_NONE);
    const auto nok = TtdKeeper::instance().fetch_nok_reason();
    cout << system_clock::now() << " W nexoid-fat    "
         << __func__ << ": " << ter << ' ' << nok << endl;
    s_nexui->interaction(create_maintenance_info(ter, nok));
    const auto ret = decide_what_kind_of_maintenance_is_required(ter);
    s_nexui->interaction({ NexuiRequest::Api::output, {map_maintenance_type_tostring(ret)} });
    return ret;
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
