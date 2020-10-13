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

static ScapiResult
handle_bad_response(const scapi::Response& rsp) {
    if (rsp.index() != 0) {
        throw runtime_error("Bad response");
    }
    const auto& nak = get<scapi::Nak>(rsp);
    ttd.nokReason = nak.nokReason;
    if (nak.terminalErrorReason) {
        ttd.terminalErrorIndicator = true;
        ttd.terminalErrorReason = *nak.terminalErrorReason;
    }
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
    const scapi::Request req = (scapi::UpdateInterfaces){
        .interfaceStatus = status
    };
    const auto rsp = s_scapi->interaction(req);
    return (rsp->index() == 1) ? SCAPI_OK : handle_bad_response(*rsp);
} catch (...) {
    return handle_exception();
}

extern "C" ScapiResult
scapi_Data_Output_Interaction(const size_t size, const CardholderMessage msg[]) noexcept try {
    const scapi::Request req = { };
    const auto rsp = s_scapi->interaction(req);
    return (rsp->index() == 1) ? SCAPI_OK : handle_bad_response(*rsp);
} catch (...) {
    return handle_exception();
}

extern "C" ScapiResult
scapi_Data_Print_Interaction(const PrintMessage m) noexcept try {
    const scapi::Request req = (scapi::PrintMessage){
        .type = m,
        .extraData = nullptr
    };
    const auto rsp = s_scapi->interaction(req);
    return (rsp->index() == 1) ? SCAPI_OK : handle_bad_response(*rsp);
} catch (...) {
    return handle_exception();
}

extern "C" ScapiResult
scapi_Data_Entry_Interaction(size_t size, const CardholderMessage msg[]) noexcept try {
    const scapi::Request req = { };
    const auto rsp = s_scapi->interaction(req);
    if (rsp->index() != 2) {
        return handle_bad_response(*rsp);
    }
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
