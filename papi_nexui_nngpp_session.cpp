#include "papi_nexui_nngpp_session.hpp"

#include "exceptions.hpp"
#include "utils.hpp"

#include <nngpp/nngpp.h>
#include <nngpp/protocol/req0.h>

#include <vector>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sys/syscall.h>

using namespace papi::nngpp;
using namespace nng;
using namespace std;
using namespace chrono;
using papi::NexuiResponse;
using papi::NexuiRequest;

static ostream&
operator << (ostream& os, const buffer& rhs) {
    return os << reinterpret_cast<const char*>(rhs.data());
}

struct NexuiSession::Impl {
    socket interaction_socket;
    const char* const addr = "ipc:///tmp/nexui";
    const milliseconds send_timeout = 1s;
    const milliseconds recv_timeout = 1min + 5s;
    const char* const name = "papi_ui";

    Impl(void);
    ~Impl(void) = default;

    NexuiResponse interaction(const NexuiRequest& req);

    vector<unsigned char> exch(const vector<unsigned char>&);

    class ExchangeLogger {
        const NexuiSession::Impl& ctx;
        ostream& out;
    public:
        vector<unsigned char> rsp;

        ExchangeLogger(const NexuiSession::Impl& s, const vector<unsigned char>& rq, ostream& os);
        ~ExchangeLogger();
    };

    inline struct ExchangeLogger make_exchange_logger(const vector<unsigned char>& rq, ostream&& os) {
        return ExchangeLogger(*this, rq, os);
    }

    inline struct ExchangeLogger make_exchange_logger(const vector<unsigned char>& rq, ostream& os) {
        return ExchangeLogger(*this, rq, os);
    }
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
    ss << R"({"source": {"type": "pap"}, "payload": [{"api": ")" << req.api << R"(", "line": [ ")" << join(req.lines, R"(", ")") << R"(" ]}]})";
    const auto s = ss.str();
    return vector<unsigned char>(s.begin(), s.end());
}

static NexuiResponse
decode_nexui_response(vector<unsigned char> rsp) {
    // TODO: Implement proper JER decoder
    return {};
}

static ostream& log_preamble(ostream& os, const char* const name) {
    static const auto tid = syscall(SYS_gettid);
    return os << system_clock::now() << " D nexoid-fat    t:" << tid << " n:" << name;
}

NexuiSession::Impl::ExchangeLogger::ExchangeLogger(const NexuiSession::Impl& s, const vector<unsigned char>& rq, ostream& os)
    : ctx(s), out(os) {
    log_preamble(out, ctx.name)
        << " prot:" << get_opt_protocol_name(ctx.interaction_socket)
        << ' ' << std::string(rq.begin(), rq.end()) << endl;
}

NexuiSession::Impl::ExchangeLogger::~ExchangeLogger(void) noexcept try {
    log_preamble(out, ctx.name)
        << " peer:" << get_opt_peer_name(ctx.interaction_socket)
        << ' ' << std::string(rsp.begin(), rsp.end()) << endl;
} catch (...) {
    // FIXME: Do something better then suppressing all possible errors in ExchangeLogger destructor
}

NexuiSession::Impl::Impl(void) : interaction_socket(req::open()) {
    set_opt_recv_timeout(interaction_socket, integer_cast<nng_duration>(recv_timeout.count()));
    set_opt_send_timeout(interaction_socket, integer_cast<nng_duration>(send_timeout.count()));
    set_opt_socket_name(interaction_socket, name);
    interaction_socket.dial(addr);
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
    ExchangeLogger l = make_exchange_logger(rq, cout);
    const auto rs = exch(rq);
    l.rsp = rs; // FIXME: Avoid unnecessary copy just for logging
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
