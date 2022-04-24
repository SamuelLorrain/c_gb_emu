#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ram.h"

void debugRam(uint8_t* ram, uint16_t addr, int offset) {
    int i;
    for(i = -offset; i < offset; i++) {
        printf("0x%x : 0x%x\n", addr+i, ram[addr+i]);
    }
}

uint8_t* newRam(size_t size) {
    uint8_t* ram = (uint8_t*)malloc(sizeof(uint8_t)* size);
    memset(ram, 0, size);
    return ram;
}
