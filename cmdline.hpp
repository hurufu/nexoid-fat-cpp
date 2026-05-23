#pragma once

#include <string>
#include <stdexcept>

/// Parsed command-line configuration.
struct CmdlineConfig {
    /// Address nexoid-cpp dials to reach the SCAPI rep socket (--req-ipc).
    std::string req_ipc;
    /// Address nexoid-cpp listens on for incoming notifications (--ntf-ipc).
    std::string ntf_ipc;
};

/// Parse argc/argv into a CmdlineConfig.
/// Throws std::invalid_argument on unknown flags or missing values.
CmdlineConfig parse_cmdline(int argc, char* argv[]);

/// Store the parsed configuration; called once from main().
void set_cmdline(CmdlineConfig cfg);

/// Retrieve the global configuration; called by session constructors.
const CmdlineConfig& get_cmdline();
