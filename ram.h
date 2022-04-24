#ifndef RAM_H
#define RAM_H
#include <stdint.h>
#include <stdlib.h>

uint8_t* newRam(size_t size);
void debugRam(uint8_t* ram, uint16_t addr, int offset);

#endif
