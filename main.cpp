#include "tostring.hpp"
#include "utils.hpp"

extern "C" {

#include <nexoid/pklr.h>
#include <nexoid/NexoFast.h>

}

#include <iostream>

using namespace std;
using namespace chrono;

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

static char
map_ProcedureResultToLogLevel(const enum ProcedureResult r) noexcept {
    switch (r) {
        case PR_TERMINATE:
            return 'I';
        case PR_SHUTDOWN:
        case PR_REBOOT:
            return 'W';
        default:
            return 'E';
    }
}

int main() {
    const enum ProcedureResult pRes = Main();
    cout << system_clock::now() << ' '
         << map_ProcedureResultToLogLevel(pRes) << " nexoid-cpp    "
         << __func__ << ": pRes: " << (int)pRes << ' ' << pRes << endl;
    return map_ProcedureResultToExitCode(pRes);
}
