#ifndef MOUSE_H
#define MOUSE_H

#include <stdint.h>

void mouse_init();
void mouse_write(uint8_t byte);
uint8_t mouse_read();
void init_mouse();


#endif
