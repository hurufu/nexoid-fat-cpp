extern "C" {
#include <nexoid/scapi.h>
#include <nexoid/gtd.h>
#include <nexoid/dmapi.h>
}

enum ScapiResult
scapi_Data_Entry_Interaction(size_t size, const enum CardholderMessage msg[]) {
    return SCAPI_NOK;
}

enum ScapiResult
scapi_Update_Interfaces(const enum InterfaceStatus status) {
    return SCAPI_NOK;
}

enum ScapiResult
scapi_Wait_For_Event(void) {
    return SCAPI_NOK;
}

bool
scapi_Authorise_Service(void) {
    return false;
}

enum ScapiResult
scapi_Data_Output_Interaction(const size_t size, const enum CardholderMessage msg[]) {
    return SCAPI_NOK;
}

enum ScapiResult
scapi_Data_Print_Interaction(const enum PrintMessage m) {
    return SCAPI_NOK;
}
