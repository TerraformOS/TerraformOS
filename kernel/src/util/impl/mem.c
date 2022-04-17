#include <util/mem.h>


void memzero(void* dest, size_t n) {
    char* dst = (char*)dest;

    for (int i = 0; i < n; ++i) {
        dst[i] = 0x0;
    }
}
