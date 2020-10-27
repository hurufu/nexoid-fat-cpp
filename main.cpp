#include "tostring.hpp"

extern "C" {

#include <nexoid/pklr.h>
#include <nexoid/NexoFast.h>

}

#include <iostream>

using namespace std;

int main() {
    const enum ProcedureResult pRes = Main();
    cout << __FILE__ << ':' << __LINE__ << '@' << __PRETTY_FUNCTION__ << " pRes: " << (int)pRes << ' ' << pRes << endl;
    return 0;
}
