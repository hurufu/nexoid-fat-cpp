#include "papi_nexui_dummy_session.hpp"
#include <iostream>

using namespace std;
using namespace papi::dummy;
using papi::NexuiResponse;
using papi::NexuiRequest;

NexuiResponse
NexuiSession::interaction(const NexuiRequest& req) {
    for (string line : req.lines)
        cerr << line << '\n';
    cerr << endl;
    return {};
}
