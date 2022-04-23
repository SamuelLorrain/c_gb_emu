#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

void loadBootRom(cpu* c) {
    FILE* file = fopen("DMG_ROM.bin", "rb");
    fseek(file, 0, SEEK_END);
    long fileLength = ftell(file);
    rewind(file);
    fread(c->ram, 1, fileLength, file);
}

int main() {
    long count = 0;
    cpu c;
    int i;
    for(i = 0; i < RAM_SIZE; i++) {
        c.ram[i] = i % 0xff;
    }
    loadBootRom(&c);

    int error = 0;
    for(i = 0; i < 30000; i++) {
        error = tickCpu(&c);
        /* count++; */
        if (error) {
            break;
        }
    }

    debugRegisters(&c);
    printf("%d\n", i);

    return 0;
}
