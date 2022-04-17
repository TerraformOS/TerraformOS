#include <drivers/ps2/mouse.h>
#include <arch/io/io.h>


static void mouse_wait_send() {
    uint64_t timeout = 100000;

    while (timeout--) {
        if (!(inportb(0x64) & (1 << 1))) {
            return;
        }
    }
}


static void mouse_wait_recv() {
    uint64_t timeout = 100000;

    while (timeout--) {
        if (inportb(0x64) & 1) {
            return;
        }
    }
}


void mouse_write(uint8_t byte) {
    mouse_wait_send();
    outportb(0x64, 0xD4);
    mouse_wait_send();
    outportb(0x60, byte);
}


uint8_t mouse_read() {
    mouse_wait_recv();
    return inportb(0x60);
}


void init_mouse() {
    // Enable mouse interrupts.
    mouse_wait_send();
    outportb(0x64, 0x20);

    mouse_wait_recv();
    uint8_t status = inportb(0x60) | 2;
    mouse_wait_send();
    outportb(0x64, 0x60);
    mouse_wait_send();
    outportb(0x60, status);

    // Tell mouse to use default settings.
    mouse_write(0xF6);
    mouse_read();       // ACK.

    // Enable mouse.
    mouse_write(0xF4);
    mouse_read();       // ACK.
}
