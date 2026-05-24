extern "C" {
#include "nexoid-ed/include/pklr.h"
}

// TODO: This configuration should be provided by SCAP during initialization.
// I think that we should wait somewhere during Proprietary_Startup_Sequence
// for SCAP to send us those values. At least we should receive information
// if "build candidate list" or "application selection" are built-in.
// In this way nexoid can be generic and work with different SCAPs without
// recompilation.
struct PklrConfiguration pklr = {
    .buildCandidateList = true
};
