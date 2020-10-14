#pragma once

extern "C" {
#include <nexoid/scapi.h>
}

ScapiResult scapi_Initialize(void) noexcept;
ScapiResult scapi_Finalize(void) noexcept;
