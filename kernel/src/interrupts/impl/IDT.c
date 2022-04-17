#include <interrupts/IDT.h>

static struct IDTVector idt[256];
static struct IDTR idtr;


void idt_set_vector(uint8_t vector, void* isr) {
    struct IDTVector* vec = &idt[vector];
    uint64_t addr = (uint64_t)isr;
    vec->isr_low16 = addr & 0xFFFF;
    vec->isr_middle16 = (addr >> 16) & 0xFFFF;
    vec->isr_high32 = (addr >> 32) & 0xFFFFFFFF;
    vec->kernel_cs = 0x08;
    vec->reserved = 0;
    vec->reserved2 = 0;
    vec->attr = 0xF;
    vec->zero = 0;
    vec->dpl = 0;
    vec->p = 1;
}


void idt_install() {
    idtr.limit = sizeof(struct IDTVector) * 256 - 1;
    idtr.base = (uint64_t)&idt;
    __asm__ __volatile__("lidt %0" :: "memory" (idtr));
}
