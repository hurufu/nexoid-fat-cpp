extern "C" {
#include "nexoid-ed/include/scapi.h"
#include "nexoid-ed/include/gtd.h"
}

#include <random>
#include <variant>

using namespace std;

namespace nexoid {
    struct Randapi {
        typedef variant<uint8_t, TerminalErrorReason> Generate_Random_Number_Result;

        virtual ~Randapi() noexcept = default;
        virtual Generate_Random_Number_Result Generate_Random_Number(const uint8_t upperLimit) noexcept = 0;
    };
}

class E1kdKeeper {
public:
    TerminalErrorReason handle_exception(const char* proc_name) {
        return TE_HARDWARE_ERROR;
    }
};

class StdRandapi : public nexoid::Randapi {
    E1kdKeeper& keeper;
public:
    StdRandapi(E1kdKeeper& k) : keeper(k) { }
    ~StdRandapi() = default;
    Generate_Random_Number_Result Generate_Random_Number(const uint8_t upperLimit) noexcept override;
};

nexoid::Randapi::Generate_Random_Number_Result
StdRandapi::Generate_Random_Number(const uint8_t upperLimit) noexcept try {
    random_device rd;
    uniform_int_distribution<uint8_t> dist(0, upperLimit);
    return dist(rd);
} catch (...) {
    return keeper.handle_exception(__func__);
}

enum ScapiResult
randapi_Generate_Random_Number(const uint8_t upperLimit, uint8_t* const randomNumber) noexcept try {
    random_device rd;
    uniform_int_distribution<uint8_t> dist(0, upperLimit);
    *randomNumber = dist(rd);
    return SCAPI_OK;
} catch (...) {
    ttd.terminalErrorReason = TE_HARDWARE_ERROR;
    ttd.terminalErrorIndicator = true;
    return SCAPI_NOK;
}

enum ScapiResult
randapi_Generate_Random_Bytes(const size_t size, uint8_t randomBytes[]) noexcept {
    return SCAPI_NOK;
}
