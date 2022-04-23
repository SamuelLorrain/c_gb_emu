#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include <stdbool.h>

#define RAM_SIZE 0xffff

/*
* Thanks to Cinoop
* https://cturt.github.io/cinoop.html
*/
typedef struct {
    struct {
        union {
            struct {
                uint8_t f;
                uint8_t a;
            };
            uint16_t af;
        };
    };
    struct {
        union {
            struct {
                uint8_t c;
                uint8_t b;
            };
            uint16_t bc;
        };
    };
    struct {
        union {
            struct {
                uint8_t e;
                uint8_t d;
            };
            uint16_t de;
        };
    };
    struct {
        union {
            struct {
                uint8_t l;
                uint8_t h;
            };
            uint16_t hl;
        };
    };
    uint16_t sp;
    uint16_t pc;
} registers;

typedef struct {
    registers reg;
    uint8_t ram[RAM_SIZE];
} cpu;

typedef struct {

} instruction;

int fetchCurrent(cpu* c);
int fetchNext(cpu* c);
int tickCpu(cpu* c);

void push_16(cpu* c, uint16_t value);
/* void pop(cpu* c); */

void setFlag(cpu* c, char flag, bool value);
bool isFlagSet(cpu* c, char flag);

void debugRegisters(cpu* c);

#endif
