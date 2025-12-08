#pragma once

#include "scapi_session.hpp"
#include "nexoid-ed/include/scapi.h"
#include <memory>

/** Reference to SCAPI Session singleton.
 *
 *  Initialized during papi_Proprietary_Startup_Sequence
 */
extern ::std::unique_ptr<::scapi::Session> s_scapi;

ScapiResult scapi_Initialize(void) noexcept;
