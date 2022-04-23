#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "cpu.h"
#include "instruction.h"

#define dRegMacro(c, REGISTER_NAME) printf("%s : 0x%x\n", REGISTER_NAME, c->reg.REGISTER_NAME)

int fetchCurrent(cpu* c) {
    return c->ram[c->reg.pc];
}

int fetchNext(cpu* c) {
    c->reg.pc += 1;
    return c->ram[c->reg.pc];
}

int tickCpu(cpu* c) {
    uint8_t currentInstruction = fetchCurrent(c);
    int x = execute(c, currentInstruction);
    if (x != 0) {
        return 2;
    }

    c->reg.pc += 1;
    return 0;
}

void push(cpu* c, uint16_t value) {
    uint8_t vh = (value >> 8) & 0xff;
    uint8_t vl = value & 0xff;
    uint16_t addr = c->reg.pc;
    c->reg.sp = addr - 1;
    c->ram[c->reg.sp] = vh;
    c->reg.sp = addr - 2;
    c->ram[c->reg.sp] = vl;
}

void debugRegisters(cpu* cpu) {
    char* a = "a";
    char* b = "b";
    char* c = "c";
    char* d = "d";
    char* e = "e";
    char* f = "f";
    char* h = "h";
    char* l = "l";
    char* sp = "sp";
    char* pc = "pc";
    dRegMacro(cpu, a);
    dRegMacro(cpu, b);
    dRegMacro(cpu, c);
    dRegMacro(cpu, d);
    dRegMacro(cpu, e);
    dRegMacro(cpu, f);
    dRegMacro(cpu, h);
    dRegMacro(cpu, l);
    dRegMacro(cpu, sp);
    dRegMacro(cpu, pc);
}


void setFlag(cpu* c, char flag, bool value) {
    uint8_t bitOffset = 0;
    switch(flag) {
        case ('z'):
            bitOffset = 7;
            break;
        case ('n'):
            bitOffset = 6;
            break;
        case ('h'):
            bitOffset = 5;
            break;
        case ('c'):
            bitOffset = 4;
            break;
        default:
            printf("Error: trying to access unknown flag");
            return;
            break;
    }
    if (value) {
        c->reg.f |= (1<<bitOffset);
    }
    else {
        c->reg.f &= 0xff & (0<<bitOffset);
    }
}

bool isFlagSet(cpu* c, char flag) {
    uint8_t flags = c->reg.f;
    uint8_t bitOffset = 0;
    switch(flag) {
        case ('z'):
            bitOffset = 7;
            break;
        case ('n'):
            bitOffset = 6;
            break;
        case ('h'):
            bitOffset = 5;
            break;
        case ('c'):
            bitOffset = 4;
            break;
        default:
            printf("Error: trying to access unknown flag");
            return false;
            break;
    }
    return (c->reg.f & (1 << bitOffset)) >> bitOffset  == 1;
}
