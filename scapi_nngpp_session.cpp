#include "scapi_nngpp_session.hpp"

using namespace ::std;
using namespace ::scapi::nngpp;

Session::Session(void) {
}

unique_ptr<::scapi::Response>
Session::interaction(const ::scapi::Request& r) {
    return make_unique<::scapi::Response>();
}

unique_ptr<::scapi::Notification>
Session::notification(void) {
    return make_unique<::scapi::Notification>();
}
