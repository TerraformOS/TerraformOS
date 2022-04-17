#include <arch/serial/serial.h>
#include <arch/io/io.h>


void init_serial() {
    outportb(COM1 + 1, 0x00);
    outportb(COM1 + 3, 0x80);
    outportb(COM1 + 0, 0x03);
    outportb(COM1 + 1, 0x00);
    outportb(COM1 + 3, 0x03);
    outportb(COM1 + 2, 0xC7);
    outportb(COM1 + 4, 0x0B);

    // serial_log(INFO, "Serial initialized\n");
    // kernel_log(INFO, "Serial initialized\n");
}


static int is_transmit_empty() {
    return inportb(COM1 + 5) & 0x20;
}


void write_serial(char c) {
    while (!(is_transmit_empty()));
    outportb(COM1, c);
}
