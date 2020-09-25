extern "C" {
#include <nexoid/dmapi.h>
}

enum DmapiResult
dmapi_init(void) {
    return DMAPI_NOK;
}

void
dmapi_dtor(void) {
}

__attribute__(( malloc ))
void*
dmapi_malloc(const size_t s) {
    return NULL;
}

void
dmapi_free(void* const ptr) {
}
