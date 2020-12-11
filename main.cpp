#include "tostring.hpp"

extern "C" {

#include <nexoid/pklr.h>
#include <nexoid/NexoFast.h>

}

#include <iostream>

using namespace std;

static int
map_ProcedureResultToExitCode(const enum ProcedureResult r) noexcept {
    switch (r) {
        case PR_TERMINATE: return 0;
        case PR_SHUTDOWN: return 1;
        case PR_REBOOT: return 2;
        default:
            break;
    }
    return 255;
}

int main() {
    const enum ProcedureResult pRes = Main();
    cout << __FILE__ << ':' << __LINE__ << '@' << __PRETTY_FUNCTION__ << " pRes: " << (int)pRes << ' ' << pRes << endl;
    return map_ProcedureResultToExitCode(pRes);
}
