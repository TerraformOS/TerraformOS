#include <drivers/video/FrameBuffer.h>
#include <drivers/ps2/mouse.h>
#include <debug/log.h>
#include <interrupts/IDT.h>
#include <interrupts/exceptions.h>
#include <arch/memory/GDT.h>
#include <arch/memory/memory.h>
#include <arch/memory/vmm.h>
#include <arch/io/io.h>
#include <arch/io/legacy-pic.h>
#include <proc/TSS.h>


canvas_t canvas = {
    .x = 0,
    .y = 2,
    .prevX = 0
};

void log(const char* format, STATUS status, ...) {
    va_list ptr;

    size_t args = 0;

    for (int i = 0; i < strlen(format); ++i) {
        if (format[i] == '%') {
            switch (format[i + 1]) {
                case 's':
                case 'd':
                    ++args;
                    i += 2;
                    break;
            }
        }
    }

    // A hacky way to get rid of the warning: second parameter of 'va_start' not last named argument
    STATUS s = status;
    status ^= status;
    status += args;
    va_start(ptr, status);

    int color = 0xFFFFFF;

    switch (s) {
        case S_WARNING:
            color = 0x800080;
            kwrite(&canvas, "[WARNING] ", color);
            break;
        case S_INFO:
            color = 0x00FF00;
            kwrite(&canvas, "[INFO] ", color);
            break;
        case S_PANIC:
            color = 0xFF0000;
            kwrite(&canvas, "** KERNEL PANIC **\n", color);
            break;
        case S_CRITICAL:
            color = 0xFF0000;
            kwrite(&canvas, "[CRITICAL] ", color);
            break;
    }

    for (int i = 0; i < strlen(format); ++i) {
        if (format[i] == '%') {
            switch (format[i + 1]) {
                case 's':
                    {
                        const char* arg = va_arg(ptr, const char*);

                        for (int j = 0; j < strlen(arg); ++j) {
                            char terminated[2] = {arg[j], 0x0};
                            kwrite(&canvas, terminated, color);
                        }

                        ++i;

                    }

                    continue;
                case 'd':
                    {
                        int arg = va_arg(ptr, int);

                        kwrite(&canvas, dec2str(arg), color);
                    }
                    ++i;
                    continue;
                case 'x':
                    {
                        int arg = va_arg(ptr, int);
                        kwrite(&canvas, hex2str(arg), color);
                        ++i;
                        continue;
                    }
            }
        }

        char terminated[2] = {format[i], 0x0};
        kwrite(&canvas, terminated, color);
    }
}


__attribute__((interrupt)) static void done(struct InterruptFrame* frame) {
    log("I <3 cats!!111!\n", S_INFO);
    PIC_sendEOI(12);
    mouse_read();
    inportb(0x60);
    inportb(0x64);
}

__attribute__((interrupt)) static void dummy(struct InterruptFrame*) {
    PIC_sendEOI(1);
}


static void init(meminfo_t meminfo) { 
    load_gdt();
    idt_set_vector(0x0, div0_handler);
    idt_set_vector(0x1, debug_exception);
    idt_set_vector(0x4, overflow_exception);
    idt_set_vector(0x5, bre_exception);
    idt_set_vector(0x6, invalid_opcode_exception);
    idt_set_vector(0x7, dev_not_avail_exception);
    idt_set_vector(0x8, double_fault);
    idt_set_vector(0xA, invalid_tss_exception);
    idt_set_vector(0xB, snp_exception);
    idt_set_vector(0xC, stackseg_fault);
    idt_set_vector(0xD, gpf);
    idt_set_vector(0xE, page_fault);
    idt_set_vector(0x2C, done);
    idt_set_vector(0x21, done);
 
    serial_log("Setting up PIC..\n", S_INFO);
    log("Setting up PIC..\n", S_INFO);
    init_pic(); 

    idt_install();
    serial_log("Setting up VMM..\n", S_INFO);
    log("Setting up VMM..\n", S_INFO);
    vmm_init(meminfo);

    serial_log("Setting up TSS..\n", S_INFO);
    log("Setting up TSS..\n", S_INFO);

    __asm__ __volatile__("sti");

    serial_log("Setting up mouse..\n", S_INFO);
    log("Setting up mouse..\n", S_INFO);
    init_mouse();

    outportb(PIC1_DATA, 0x0 | 1);
    outportb(PIC2_DATA, 0x0);


    init_tss();
}


void _context_switch();


int _start(framebuffer_t* lfb, psf1_font_t* font, meminfo_t meminfo) {
    canvas.font = font;
    canvas.lfb = lfb;

    init(meminfo);

    log("Hello, World!\n", S_INFO);

    while (1) {
        __asm__ __volatile__("hlt");
    }
}
