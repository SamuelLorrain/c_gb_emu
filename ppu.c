#include "ppu.h"

/* PPU REGISTERS */

/**
* ===================================================
* FF41 - Bits 0 & 1
* ===================================================
*/
void setLcdStatusMode(ppu* p, ppu_state_mode s) {
    uint8_t v = p->ram[LCD_STATUS];
    v &= 0b11111100;
    switch (s) {
        case hblank:
            v |= 0x0;
            break;
        case vblank:
            v |= 0x1;
            break;
        case oamsearch:
            v |= 0x2;
            break;
        case pixeltransfer:
            v |= 0x3;
            break;
    }
    p->ram[LCD_STATUS] = v;
}

ppu_state_mode getLcdStatusMode(ppu* p) {
    uint8_t v = p->ram[LCD_STATUS];
    v &= 0b00000011;
    switch (v) {
        case 0b0:
            return hblank;
        case 0b01:
            return vblank;
        case 0b10:
            return oamsearch;
        case 0b11:
            return pixeltransfer;
    }
    return hblank;
}

void tickPpu(ppu* p) {
    if (!(isLcdOnMacro(p))) {
        return;
    }
    // LY == LYC ?
    if (p->ram[LY] == p->ram[LYC]) {
        p->ram[LCD_STATUS] = (p->ram[LCD_STATUS] & 0b11111011) | 0b100;
    } else {
        p->ram[LCD_STATUS] = p->ram[LCD_STATUS] & 0b11111011;
    }
}
