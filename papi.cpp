#include "scapi_socket_session.hpp"
#include "scapi_internal.hpp"

extern "C" {
#include <nexoid/papi.h>
#include <nexoid/gtd.h>
}

#include <cstring>
#include <stdexcept>
#include <iostream>

using namespace std;

static PapiResult
papi_handle_exception(void) noexcept try {
    ttd.terminalErrorIndicator = true;
    throw;
} catch (const exception& e) {
    ttd.terminalErrorReason = TE_UNSPECIFIED;
    cerr << __FILE__ << ':' << __LINE__ << '@' << __func__ << " Generic exception suppressed: " << e.what() << endl;
    return PAPI_NOK;
} catch (const libsocket::socket_exception& e) {
    ttd.terminalErrorReason = TE_COMMUNICATION_ERROR;
    cerr << __FILE__ << ':' << __LINE__ << '@' << __func__ << " Connectivity related exception suppressed: " << strerror(e.err) << '\n' << e.mesg << endl;
    return PAPI_NOK;
} catch (...) {
    ttd.terminalErrorReason = TE_UNSPECIFIED;
    cerr << __FILE__ << ':' << __LINE__ << '@' << __func__ << " Unexpected exception suppressed" << endl;
    return PAPI_NOK;
}

enum PapiResult
papi_Proprietary_Startup_Sequence(void) noexcept try {
    if (scapi_Initialize() != SCAPI_OK) {
        cout << "SCAPI Initialization failed" << endl;
        return PAPI_NOK;
    }
    return PAPI_OK;
} catch (...) {
    return papi_handle_exception();
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
