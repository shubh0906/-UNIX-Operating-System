#ifndef COMMON_H
#define COMMON_H
#include <sys/defs.h>

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
void outl(uint16_t port, uint32_t value);
uint32_t inl(uint16_t port);
#endif
