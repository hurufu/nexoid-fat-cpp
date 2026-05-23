#pragma once

#include "papi_interface.hpp"
#include "papi_nexui_session.hpp"
#include "scapi_session.hpp"
#include "ttd_keeper.hpp"
#include <memory>
#include <experimental/propagate_const>

namespace papi {

    class ProprietaryLogic : public ::papi::ProprietaryApi {
        struct Impl;
        ::std::experimental::propagate_const<::std::unique_ptr<Impl>> pimpl;
    public:

        explicit ProprietaryLogic(::std::unique_ptr<::papi::NexuiSession> nexui, TtdKeeper& ttd);
        ~ProprietaryLogic(void) noexcept;

        enum PapiResult Proprietary_Startup_Sequence() noexcept override;
        enum ProcedureResult Diagnostics_Maintenance_Recovery() noexcept override;
        enum PapiResult Specific_Processing_Based_On_Pan() noexcept override;
        enum PapiCvmResult Proprietary_Cvm_Condition_Code_Processing(const struct CvRule) noexcept override;
    };

}
