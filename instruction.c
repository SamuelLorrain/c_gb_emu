#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "cpu.h"
#include "instruction.h"

#define hasCarryFlag(oldV, v) ((((oldV & 0xf) + (v & 0xf)) & 0x10) == 0x10)

void handleCBinstruction(cpu* c) {
    uint8_t currentInstruction = fetchCurrent(c);
    switch(currentInstruction) {
        case (0x7c):
            setFlag(c, 'n', 0);
            setFlag(c, 'h', 1);
            if ((c->reg.h & (1 << 7)) == 0) {
                setFlag(c, 'z', 1);
            } else {
                setFlag(c, 'z', 0);
            }
            break;
        default:
            printf("Error: 0xcb 0x%x instruction is not implemented\n", currentInstruction);
            exit(2);
            break;
    }
}

void execute(cpu* c, uint8_t ins) {
    uint8_t x;
    uint8_t y;
    uint16_t addr;
    uint16_t v;
    int8_t offset;

    switch(ins) {
        case (0x04):
            x = c->reg.b;
            c->reg.b += 1;
            setFlag(c, 'n', 1);
            if(hasCarryFlag(x, c->reg.b)) {
                setFlag(c, 'h', 1);
            }
            if (c->reg.b == 0) {
                setFlag(c, 'z', 1);
            } else {
                setFlag(c, 'z', 0);
            }
            break;
        case (0x05):
            x = c->reg.b;
            c->reg.b -= 1;
            setFlag(c, 'n', 1);
            if(hasCarryFlag(x, c->reg.b)) {
                setFlag(c, 'h', 1);
            }
            if (c->reg.b == 0) {
                setFlag(c, 'z', 1);
            } else {
                setFlag(c, 'z', 0);
            }
            break;
        case (0x06):
            x = fetchNext(c);
            c->reg.b = x;
            break;
        case (0x0c):
            setFlag(c, 'n', 0);
            x = c->reg.c;
            c->reg.c += 1;
            if (c->reg.c == 0) {
                setFlag(c, 'z', 0);
            }
            if (hasCarryFlag(x, c->reg.c)) {
                setFlag(c, 'h', 0);
            }
            break;
        case (0x0d):
            x = c->reg.c;
            c->reg.c -= 1;
            setFlag(c, 'n', 1);
            if(hasCarryFlag(x, c->reg.c)) {
                setFlag(c, 'h', 1);
            }
            if (c->reg.c == 0) {
                setFlag(c, 'z', 1);
            } else {
                setFlag(c, 'z', 0);
            }
            break;
        case (0x0e):
            x = fetchNext(c);
            c->reg.c = x;
            break;
        case (0x11):
            x = fetchNext(c);
            y = fetchNext(c);
            c->reg.de = (y << 8) + x;
            break;
        case (0x13):
            c->reg.de = c->reg.de + 1;
            break;
        case (0x18):
            offset = (int8_t)fetchNext(c);
            c->reg.pc += offset;
            break;
        case (0x1a):
            x = c->ram[c->reg.de];
            c->reg.a = x;
            break;
        case (0x1e):
            c->reg.e = fetchNext(c);
            break;
        case (0x20): // JR NZ, addr_0007
            offset = (int8_t)fetchNext(c);
            if (!isFlagSet(c, 'z')) {
                c->reg.pc += offset;
            }
            break;
        case (0x21):
            x = fetchNext(c);
            y = fetchNext(c);
            c->reg.hl = (y << 8) + x;
            break;
        case (0x22):
            addr = c->reg.hl;
            x = c->reg.a;
            c->ram[addr] = x;
            addr += 1;
            c->reg.hl = addr;
            break;
        case (0x23):
            c->reg.hl += 1;
            break;
        case (0x28): // JR Z, r8
            offset = (int8_t)fetchNext(c);
            if (isFlagSet(c, 'z')) {
                c->reg.pc += offset;
            }
            break;
        case (0x2e):
            x = fetchNext(c);
            c->reg.l = x;
            break;
        case (0x31):
            x = fetchNext(c);
            y = fetchNext(c);
            c->reg.sp = (y << 8) + x;
            break;
        case (0x32):
            addr = c->reg.hl;
            x = c->reg.a;
            c->ram[addr] = x;
            addr -= 1;
            c->reg.hl = addr;
            break;
        case (0x3d):
            x = c->reg.a;
            c->reg.a -= 1;
            setFlag(c, 'n', 1);
            if(hasCarryFlag(x, c->reg.a)) {
                setFlag(c, 'h', 1);
            }
            if (c->reg.a == 0) {
                setFlag(c, 'z', 1);
            } else {
                setFlag(c, 'z', 0);
            }
            break;
        case (0x57):
            x = c->reg.a;
            c->reg.d = x;
            break;
        case (0x67):
            x = c->reg.a;
            c->reg.h = x;
            break;
        case (0x77):
            addr = c->reg.hl;
            x = c->reg.a;
            c->ram[addr] = x;
            break;
        case (0x7b):
            c->reg.a = c->reg.e;
            break;
        case (0xaf):
            c->reg.f = 0;
            c->reg.a = c->reg.a ^ c->reg.a;
            break;
        case (0xcb):
            c->reg.pc +=1;
            handleCBinstruction(c);
            break;
        case (0xcd): // CALL a16
            x = fetchNext(c);
            y = fetchNext(c);
            addr = (y << 8) + x;

            break;
        case (0x3e):
            x = fetchNext(c);
            c->reg.a = x;
            break;
        case (0xe0): // ld ($ff00 + c), a
            addr = 0xff00 + fetchNext(c);
            x = c->reg.a;
            c->ram[addr] = x;
            break;
        case (0xe2): // ld ($ff00 + c), a
            x = c->reg.a;
            addr = 0xff00 + c->reg.c;
            c->ram[addr] = x;
            break;
        case (0xea):
            x = fetchNext(c);
            y = fetchNext(c);
            addr = (y << 8) + x;
            c->ram[addr] = c->reg.a;
            break;
        case (0xf0):
            addr = 0xff + fetchNext(c);
            c->reg.a = c->ram[addr];
            break;
        case (0xfe):
            x = c->reg.a;
            y = fetchNext(c);
            v = x - y;
            c->reg.f = 0;
            setFlag(c, 'n', 1);
            if(v == 0) {
                setFlag(c, 'z', 1);
            }
            if ((((x & 0xf) - (y & 0xf)) & (0xf + 1)) != 0) {
                setFlag(c, 'h', 1);
            }
            if(x - y < 0) {
                setFlag(c, 'c', 1);
            }
            break;
        default:
            printf("Error: 0x%x instruction is not implemented\n", ins);
            exit(2);
            break;
    }
}
