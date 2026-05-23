#include "cmdline.hpp"

#include <string_view>
#include <string>
#include <stdexcept>

static CmdlineConfig g_cmdline_config;

void set_cmdline(CmdlineConfig cfg) {
    g_cmdline_config = std::move(cfg);
}

const CmdlineConfig& get_cmdline() {
    return g_cmdline_config;
}

CmdlineConfig parse_cmdline(int argc, char* argv[]) {
    CmdlineConfig cfg;

    auto require_next = [&](int& i, const char* name) -> std::string {
        if (++i >= argc)
            throw std::invalid_argument(std::string(name) + " requires a value");
        return argv[i];
    };

    for (int i = 1; i < argc; ++i) {
        const std::string_view arg(argv[i]);
        if      (arg == "--req-ipc") cfg.req_ipc = require_next(i, "--req-ipc");
        else if (arg == "--ntf-ipc") cfg.ntf_ipc = require_next(i, "--ntf-ipc");
        else throw std::invalid_argument("Unknown argument: " + std::string(arg));
    }

    if (cfg.req_ipc.empty())
        throw std::invalid_argument("Missing required argument: --req-ipc");
    if (cfg.ntf_ipc.empty())
        throw std::invalid_argument("Missing required argument: --ntf-ipc");

    return cfg;
}
