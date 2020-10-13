#pragma once

extern "C" {
#include <nexoid/scapi.h>
}

ScapiResult scapi_Initialize(void);
ScapiResult scapi_Finalize(void);
