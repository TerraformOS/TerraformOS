#ifndef PMM_H
#define PMM_H

#define _1KB (1 << 10)
#define _1MB (1 << 21)
#define _1GB (1 << 30)
#define RAM 64 * _1MB
#define PGE_SZ 4096
#define BMP_SZ RAM / (PGE_SZ * 8 * sizeof(unsigned int))

#include <stdint.h>
#include <stddef.h>


void* alloc_frame();


#endif
