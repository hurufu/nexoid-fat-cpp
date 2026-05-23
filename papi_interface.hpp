#pragma once

#include "nexoid-ed/include/papi.h"

namespace papi {

    class ProprietaryApi {
    public:
        virtual enum PapiResult Proprietary_Startup_Sequence() noexcept = 0;
        virtual enum ProcedureResult Diagnostics_Maintenance_Recovery() noexcept = 0;
        virtual enum PapiResult Specific_Processing_Based_On_Pan() noexcept = 0;
        virtual enum PapiCvmResult Proprietary_Cvm_Condition_Code_Processing(const struct CvRule) noexcept = 0;
    };

}
