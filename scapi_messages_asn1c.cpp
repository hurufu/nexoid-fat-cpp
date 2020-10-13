#include "scapi_messages_asn1c.hpp"

#include <ScapiNotification.h>
#include <ScapiRequest.h>
#include <ScapiResponse.h>

#include <stdexcept>

using namespace std;

vector<unsigned char>
encode(const ::scapi::socket::Request&) {
    throw runtime_error("Not implemented");
}

const ::scapi::socket::Response&
decode(const vector<unsigned char>&) {
    throw runtime_error("Not implemented");
}
