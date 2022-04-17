#ifndef IO_H
#define IO_H

unsigned char inportb(unsigned short port);
void outportb(unsigned short port, unsigned char data);
void io_wait();

#endif

