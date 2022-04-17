#ifndef VMM_H
#define VMM_H

#include <arch/memory/memory.h>
#include <stdint.h>


struct MappingTable {
    uint64_t entries[512];
};


void load_pml4(struct MappingTable* pml4);

void* alloc_page();
void map_page(uint64_t logical);
void vmm_init(meminfo_t meminfo);

#endif
