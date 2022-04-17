#ifndef IDT_H
#define IDT_H

#include <stdint.h>


struct __attribute__((packed)) IDTVector {
    uint16_t isr_low16;
    uint16_t kernel_cs;
    uint8_t ist : 3;
    uint8_t reserved : 5;
    uint8_t attr : 4;
    uint8_t zero : 1;
    uint8_t dpl : 2;
    uint8_t p : 1;
    uint16_t isr_middle16;
    uint32_t isr_high32;
    uint32_t reserved2;
};


struct __attribute__((packed)) IDTR {
    uint16_t limit;
    uint64_t base;
};


struct __attribute__((packed)) InterruptFrame {
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};


void idt_set_vector(uint8_t vector, void* isr);
void idt_install();


#endif
