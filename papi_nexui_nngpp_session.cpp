#include "papi_nexui_nngpp_session.hpp"

#include "exceptions.hpp"
#include "utils.hpp"

#include <nngpp/nngpp.h>
#include <nngpp/protocol/req0.h>

#include <vector>
#include <sstream>

using namespace papi::nngpp;
using namespace nng;
using namespace std;
using papi::NexuiResponse;
using papi::NexuiRequest;

struct NexuiSession::Impl {
    socket interaction_socket;

    Impl(void);
    ~Impl(void) = default;

    NexuiResponse interaction(const NexuiRequest& req);

    vector<unsigned char> exch(const vector<unsigned char>&);
};

static const char* tostring(const ::papi::NexuiRequest::Api r) {
    switch (r) {
        case NexuiRequest::Api::output: return "output";
        case NexuiRequest::Api::entry: return "entry";
        case NexuiRequest::Api::ssn: return "ssn";
    }
    throw bad_mapping(static_cast<int>(r), "papi::NexuiRequest::Api");
}

static inline ::std::ostream&
operator << (::std::ostream& os, const papi::NexuiRequest::Api& x) {
    return os << tostring(x);
}

static vector<unsigned char>
encode_nexui_request(const NexuiRequest& req) {
    stringstream ss;
    ss << "{\"source\": {\"type\": \"pap\"}, \"payload\": [{\"api\": \"" << req.api << "\", \"line\": [\"" << join(req.lines, "\", \"") << "\"]}]}";
    const auto s = ss.str();
    cout << "enc: " << s << endl;
    return vector<unsigned char>(s.begin(), s.end());
}

static NexuiResponse
decode_nexui_response(vector<unsigned char>) {
    return {};
}

NexuiSession::Impl::Impl(void) : interaction_socket(req::open()) {
    set_opt_recv_timeout(interaction_socket, 3 * 1000);
    set_opt_send_timeout(interaction_socket, 1 * 1000);
    interaction_socket.dial("ipc:///tmp/nexui");
}

vector<unsigned char>
NexuiSession::Impl::exch(const vector<unsigned char>& b) {
    const view vreq(b.data(), b.size());
    interaction_socket.send(vreq);
    buffer nrsp = interaction_socket.recv();
    vector<unsigned char> rsp(nrsp.data<unsigned char>(), nrsp.data<unsigned char>() + nrsp.size());
    return rsp;
}

NexuiResponse
NexuiSession::Impl::interaction(const NexuiRequest& req) {
    const auto rq = encode_nexui_request(req);
    const auto rs = exch(rq);
    return decode_nexui_response(rs);
}

NexuiSession::NexuiSession(void) :
    pimpl(make_unique<NexuiSession::Impl>()) {
}

NexuiSession::~NexuiSession(void) noexcept = default;

NexuiResponse
NexuiSession::interaction(const NexuiRequest& req) {
    return pimpl->interaction(req);
}
