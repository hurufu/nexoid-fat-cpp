#include "ttd_keeper.hpp"

extern "C" {
#include <nexoid/hapi.h>
}

enum HapiResult
hapi_Online_Approval_Request(void) {
    // FIXME: Use single method update(OnlineAuthData), so TtdKeeper will have more abilities to check for consistency
#   if 0
    TtdKeeper::instance().update(N_DATA_ERROR);
    return HAPI_NOK;
#   else
    TtdKeeper::instance().update(T_APPROVED);
    TtdKeeper::instance().update(ARC_ONLINE_APPROVED);
    return HAPI_OK;
#   endif
}

enum HapiResult
hapi_Search_Transactions(const struct SearchLogCriteria* const slc) {
    return HAPI_NOK;
}

enum HapiResult
hapi_SearchReservationsByPan(void) {
    return HAPI_NOK;
}

enum HapiResult
hapi_Online_Request_to_Dcc_Provider(void) {
    return HAPI_NOK;
}

enum HapiResult
hapi_Transaction_Data_Storage(void) {
    return HAPI_OK;
}

enum HapiResult
hapi_Transaction_Finalisation(void) {
    return HAPI_OK;
}

enum HapiResult
hapi_SearchReservationsByRefData(void) {
    return HAPI_NOK;
}

enum HapiResult
hapi_Status(void) {
    return HAPI_OK;
}

enum HapiResult
hapi_Log_Entry_Data_Retrieval(void) {
    return HAPI_NOK;
}
