#include "scapi_socket_session.hpp"

#include "scapi_internal.hpp"

#include <ScapiNotification.h>
#include <ScapiRequest.h>
#include <ScapiResponse.h>

extern "C" {
#include <nexoid/scapi.h>
#include <nexoid/gtd.h>
#include <nexoid/dmapi.h>
}

#include <stdexcept>
#include <vector>
#include <iostream>
#include <memory>

using namespace std;

static unique_ptr<ScapiSocketSession> s_scapi;

void*
make_interaction_request(const size_t size, const CardholderMessage msg[]) {
    return NULL;
}

static ScapiResult
handle_exception(void) noexcept try {
    ttd.terminalErrorIndicator = true;
    throw;
} catch (const exception& e) {
    ttd.terminalErrorReason = TE_UNSPECIFIED;
    return SCAPI_NOK;
} catch (...) {
    ttd.terminalErrorReason = TE_UNSPECIFIED;
    return SCAPI_NOK;
}

ScapiResult
scapi_Initialize(void) {
    return SCAPI_OK;
}

ScapiResult
scapi_Finalize(void) {
    return SCAPI_OK;
}

extern "C" ScapiResult
scapi_Update_Interfaces(const InterfaceStatus status) noexcept try {
    return SCAPI_OK;
} catch (...) {
    return handle_exception();
}

extern "C" ScapiResult
scapi_Data_Entry_Interaction(size_t size, const CardholderMessage msg[]) noexcept try {
    return SCAPI_OK;
} catch (...) {
    return handle_exception();
}

extern "C" ScapiResult
scapi_Data_Output_Interaction(const size_t size, const CardholderMessage msg[]) noexcept try {
    const auto request = make_interaction_request(size, msg);
    const auto response = s_scapi->interaction(request);
    if (!response) {
        throw runtime_error("Unexpected reponse type");
    }
    return SCAPI_OK;
} catch (...) {
    return handle_exception();
}

extern "C" ScapiResult
scapi_Data_Print_Interaction(const PrintMessage m) noexcept try {
    return SCAPI_OK;
} catch (...) {
    return handle_exception();
}

extern "C" ScapiResult
scapi_Wait_For_Event(void) noexcept try {
    return SCAPI_OK;
} catch (...) {
    return handle_exception();
}

extern "C" bool
scapi_Authorise_Service(void) noexcept {
    return true;
}
