#include <cstdlib>

extern "C" {
#include <nexoid/dmapi.h>
}

enum DmapiResult
dmapi_init(void) {
    return DMAPI_OK;
}

void
dmapi_dtor(void) {
}

__attribute__(( malloc ))
void*
dmapi_malloc(const size_t s) {
    // FIXME: Fix leaking DMAPI
    return malloc(s);
}

void
dmapi_free(void* const ptr) {
    free(ptr);
}
