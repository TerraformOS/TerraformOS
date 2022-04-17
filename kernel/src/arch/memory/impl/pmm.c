#include <arch/memory/pmm.h>


// 4096 * (8 * 4 * byte + bit);
static unsigned int bitmap[BMP_SZ] = {0};


static size_t _get_byte(const uintptr_t PHYS) {
    return PHYS / (PGE_SZ * 8 * 4);
}

static size_t _get_bit(const uintptr_t PHYS) {
    return (PHYS % (PGE_SZ * 8 * 4) / 4096);
}


void* first_fit() {
   

    return NULL;
}


void* alloc_frame() {
    for (int byte = 0; byte < BMP_SZ; ++byte) {
        unsigned int dword = bitmap[byte];
        for (int bit = 0; bit < 32; ++bit, byte >>= 1) {
            /* 00000010 (>>= 1)
             * 00000001
             * 
             * 00000001
             *    &       => 00000001
             * 00000001
             */

            if (byte & 0x1) {
                continue;
            }
 
            /*
             * 1 << bit (ex. 2)
             * 00000001
             * 00000100
             *
             * 00000000 ^ (1 << 2 [000000100] ) => 000000100
             * 
             */

            bitmap[byte] ^= (1 << bit);
            return (void*)(uint64_t)(4096 * (8 * 4 * byte + bit));
        }
    }

    return NULL;
}


void free_frame(const unsigned int PHYS) {
    unsigned int byte = _get_byte(PHYS);

    if (bitmap[byte] & (1 << _get_bit(PHYS))) {
        bitmap[byte] ^= (1 << _get_bit(PHYS));
    } 
}
