#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "cpu.h"

void execute(cpu*, uint8_t);
void handleCBinstruction(cpu* c);

#endif
