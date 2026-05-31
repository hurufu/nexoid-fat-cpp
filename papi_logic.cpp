#include "papi_logic.hpp"
#include "scapi_internal.hpp"
#include "exceptions.hpp"
#include "utils.hpp"
#include "tostring.hpp"
#include <iostream>

using namespace papi;
using namespace std;
using namespace chrono;

struct ProprietaryLogic::Impl {
    Impl(unique_ptr<NexuiSession>, TtdKeeper&);
    ~Impl() noexcept = default;
    unique_ptr<NexuiSession> nexui;
    TtdKeeper& ttd;
};

ProprietaryLogic::Impl::Impl(unique_ptr<NexuiSession> n, TtdKeeper& t) :
    nexui(move(n)), ttd(t) {
}

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
    throw bad_mapping(ter);
}

static const char*
map_maintenance_type_tostring(const enum ProcedureResult r) {
    switch (r) {
        case PR_STARTUP_SEQUENCE: return "Application restart requested";
        case PR_SHUTDOWN: return "Device shutdown requested";
        case PR_REBOOT: return "Device reboot requested";
        case PR_NOK: return "Nothing can help";
        default:
            throw bad_mapping(r);
    }
}

ProprietaryLogic::ProprietaryLogic(unique_ptr<NexuiSession> nexui, TtdKeeper& ttd) :
    pimpl(make_unique<ProprietaryLogic::Impl>(move(nexui), ttd)) {
}

ProprietaryLogic::~ProprietaryLogic(void) noexcept = default;

enum PapiResult
ProprietaryLogic::Proprietary_Startup_Sequence() noexcept try {
    pimpl->nexui->interaction({ NexuiRequest::Api::output, {"Startup"}});
    if (scapi_Initialize() != SCAPI_OK) {
        cerr << system_clock::now() << " C nexoid-fat    "
             << __func__ << ": SCAPI Initialization failed" << endl;
        pimpl->nexui->interaction({ NexuiRequest::Api::output, {"SCAPI Initialization failed"} });
        return PAPI_NOK;
    }
    return PAPI_OK;
} catch (...) {
    pimpl->ttd.handle_exception(__func__);
    return PAPI_NOK;
}

enum ProcedureResult
ProprietaryLogic::Diagnostics_Maintenance_Recovery() noexcept try {
    const auto ter = pimpl->ttd.update(TE_NONE);
    const auto nok = pimpl->ttd.fetch_nok_reason();
    cerr << system_clock::now() << " W nexoid-fat    "
         << __func__ << ": " << ter << ' ' << nok << endl;
    pimpl->nexui->interaction(create_maintenance_info(ter, nok));
    const auto ret = decide_what_kind_of_maintenance_is_required(ter);
    pimpl->nexui->interaction({ NexuiRequest::Api::output, {map_maintenance_type_tostring(ret)} });
    return ret;
} catch (...) {
    pimpl->ttd.handle_exception(__func__);
    return PR_NOK;
}

enum PapiResult
ProprietaryLogic::Specific_Processing_Based_On_Pan() noexcept try {
    return PAPI_OK;
} catch (...) {
    return PAPI_NOK;
}

enum PapiCvmResult
ProprietaryLogic::Proprietary_Cvm_Condition_Code_Processing(const struct CvRule) noexcept try {
    return PAPI_CVM_NOT_SUPPORTED;
} catch (...) {
    return PAPI_CVM_NOT_SUPPORTED;
}
