#include "ttd_keeper.hpp"
#include "utils.hpp"
#include <iostream>
#include <chrono>

extern "C" {
#include "nexoid-ed/include/hapi.h"
}

using namespace std;
using namespace chrono;

extern "C" enum HapiResult
hapi_Online_Approval_Request(void) try {
    // FIXME: Use single method update(OnlineAuthData), so TtdKeeper will have more abilities to check for consistency
#   if 0
    TtdKeeper::instance().update(N_DATA_ERROR);
    return HAPI_NOK;
#   else
    TtdKeeper::instance().update(T_APPROVED);
    TtdKeeper::instance().update(ARC_ONLINE_APPROVED);
    return HAPI_OK;
#   endif
} catch (...) {
    TtdKeeper::instance().handle_exception();
    return HAPI_NOK;
}

enum HapiResult
hapi_Search_Transactions(const struct SearchLogCriteria* const slc) {
    clog << system_clock::now() << " E nexoid-cpp    " << __func__ << " ..." << endl;
    return HAPI_NOK;
}

enum HapiResult
hapi_SearchReservationsByPan(void) {
    clog << system_clock::now() << " E nexoid-cpp    " << __func__ << " ..." << endl;
    return HAPI_NOK;
}

enum HapiResult
hapi_Online_Request_to_Dcc_Provider(void) {
    clog << system_clock::now() << " E nexoid-cpp    " << __func__ << " ..." << endl;
    return HAPI_NOK;
}

extern "C" enum HapiResult
hapi_Transaction_Data_Storage(void) try {
    clog << system_clock::now() << " W nexoid-cpp    " << __func__ << " ..." << endl;
    return HAPI_OK;
} catch (...) {
    TtdKeeper::instance().handle_exception();
    return HAPI_NOK;
}

extern "C" enum HapiResult
hapi_Transaction_Finalisation(void) try {
    clog << system_clock::now() << " W nexoid-cpp    " << __func__ << " ..." << endl;
    return HAPI_OK;
} catch (...) {
    TtdKeeper::instance().handle_exception();
    return HAPI_NOK;
}

enum HapiResult
hapi_SearchReservationsByRefData(void) {
    clog << system_clock::now() << " E nexoid-cpp    " << __func__ << " ..." << endl;
    return HAPI_NOK;
}

extern "C" enum HapiResult
hapi_Status(void) try {
    clog << system_clock::now() << " W nexoid-cpp    " << __func__ << " ..." << endl;
    return HAPI_OK;
} catch (...) {
    TtdKeeper::instance().handle_exception();
    return HAPI_NOK;
}

enum HapiResult
hapi_Log_Entry_Data_Retrieval(void) {
    clog << system_clock::now() << " E nexoid-cpp    " << __func__ << " ..." << endl;
    return HAPI_NOK;
}
