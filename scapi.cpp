#include "scapi_socket_session.hpp"
#include "scapi_internal.hpp"
#include "scapi_messages_asn1c.hpp"

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

static unique_ptr<::scapi::Session> s_scapi;

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
    s_scapi = make_unique<scapi::socket::Session>();
    return SCAPI_OK;
}

ScapiResult
scapi_Finalize(void) {
    delete s_scapi.release();
    return SCAPI_OK;
}

extern "C" ScapiResult
scapi_Update_Interfaces(const InterfaceStatus status) noexcept try {
    const scapi::Request req;
    const auto rsp = s_scapi->interaction(req);
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
    const scapi::Request request;
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
