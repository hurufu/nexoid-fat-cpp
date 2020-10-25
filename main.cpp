extern "C" {

#include <nexoid/pklr.h>
#include <nexoid/NexoFast.h>

}

#include <iostream>

using namespace std;

const char* ProcedureResult_tostring(const enum ProcedureResult r) {
    switch (r) {
        case PR_UNINITIALISED: return "PR_UNINITIALISED";
        case PR_NOT_IMPLEMENTED: return "PR_NOT_IMPLEMENTED";
        case PR_INTERNAL_INCONSISTENCY: return "PR_INTERNAL_INCONSISTENCY";
        case PR_REBOOT: return "PR_REBOOT";
        case PR_TERMINATE: return "PR_TERMINATE";
        case PR_SHUTDOWN: return "PR_SHUTDOWN";
        case PR_STARTUP_SEQUENCE: return "PR_STARTUP_SEQUENCE";
        case PR_CONTACT_TMS: return "PR_CONTACT_TMS";
        case PR_BAIL: return "PR_BAIL";
        case PR_OK: return "PR_OK";
        case PR_NOK: return "PR_NOK";
        case PR_NON_EMV: return "PR_NON_EMV";
        case PR_TAP_AGAIN: return "PR_TAP_AGAIN";
        case PR_END_APPLICATION: return "PR_END_APPLICATION";
        case PR_REFERENCE_ENTRY: return "PR_REFERENCE_ENTRY";
        case PR_ACCEPT: return "PR_ACCEPT";
        case PR_FOUND: return "PR_FOUND";
        case PR_NOT_FOUND: return "PR_NOT_FOUND";
        case PR_SUCCESSFUL: return "PR_SUCCESSFUL";
        case PR_NOT_SUCCESSFUL: return "PR_NOT_SUCCESSFUL";
        case PR_NOT_SUPPORTED: return "PR_NOT_SUPPORTED";
        case PR_MATCH: return "PR_MATCH";
        case PR_NO_MATCH: return "PR_NO_MATCH";
        case PR_DONE: return "PR_DONE";
        case PR_ONLINE: return "PR_ONLINE";
        case PR_UNABLE_TO_GO_ONLINE: return "PR_UNABLE_TO_GO_ONLINE";
        case PR_CONDITIONS_SATISFIED: return "PR_CONDITIONS_SATISFIED";
        case PR_CONDITIONS_NOT_SATISFIED: return "PR_CONDITIONS_NOT_SATISFIED";
        case PR_MANUAL_ENTRY: return "PR_MANUAL_ENTRY";
        case PR_REINITIALISE: return "PR_REINITIALISE";
        case PR_NEW_EVENT: return "PR_NEW_EVENT";
        case PR_RETRY_AFTER_CONTACTLESS: return "PR_RETRY_AFTER_CONTACTLESS";
        case PR_FALLBACK: return "PR_FALLBACK";
        case PR_PSE_UNSUCCESSFUL: return "PR_PSE_UNSUCCESSFUL";
        case PR_A: return "PR_A";
        case PR_B: return "PR_B";
        case PR_C: return "PR_C";
        case PR_D: return "PR_D";
        case PR_E: return "PR_E";
        case PR_PARTIAL_MATCH: return "PR_PARTIAL_MATCH";
        case PR_EXACT_MATCH: return "PR_EXACT_MATCH";
        case PR_SKIP: return "PR_SKIP";
        case PR_CVM_APPLICABLE: return "PR_CVM_APPLICABLE";
        case PR_CVM_NOT_APPLICABLE: return "PR_CVM_NOT_APPLICABLE";
        case PR_CVM_SUCCESSFUL: return "PR_CVM_SUCCESSFUL";
        case PR_CVM_UNSUCCESSFUL: return "PR_CVM_UNSUCCESSFUL";
        case PR_CVM_SUPPORTED: return "PR_CVM_SUPPORTED";
        case PR_CVM_NOT_SUPPORTED: return "PR_CVM_NOT_SUPPORTED";
        case PR_CVM_RETRY: return "PR_CVM_RETRY";
        case PR_MAX: return "PR_MAX";
    }
    return NULL;
}

ostream& operator<<(ostream& os, const enum ProcedureResult& pr) {
    return os << ProcedureResult_tostring(pr);
}

int main() {
    const enum ProcedureResult pRes = Main();
    cout << __FILE__ << ':' << __LINE__ << '@' << __PRETTY_FUNCTION__ << " pRes: " << (int)pRes << ' ' << pRes << endl;
    return 0;
}
