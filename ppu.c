#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ppu.h"
#define isLcdOnMacro(p) ((p->ram[LCD_CONTROL] & 0b10000000) >> 7)

ppu newPpu() {
    ppu p;
    p.onOff = false;
    p.ram = NULL;
    p.screen_surface = NULL;
    p.ticks = 0;
    p.pixels_drawn_on_current_line = 0;
    return p;
}

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
    uint8_t yDraw = 0;
    uint8_t tileLine = 0;
    uint16_t  tileMapRowAddr = 0;

    // TODO
    // SHOULD USE ON/OFF
    // TRIGGERING INSTEAD
    if (!(isLcdOnMacro(p))) {
        return;
    }

    // LY == LYC ?
    if (p->ram[LY] == p->ram[LYC]) {
        p->ram[LCD_STATUS] = (p->ram[LCD_STATUS] & 0b11111011) | 0b100;
    } else {
        p->ram[LCD_STATUS] = p->ram[LCD_STATUS] & 0b11111011;
    }

    switch(getLcdStatusMode(p)) {
        case hblank:
            if (p->ticks == 456) {
                p->ticks = 0;
                p->ram[LY] += 1;
                if (p->ram[LY] == 144) {
                    setLcdStatusMode(p, vblank);
                } else {
                    setLcdStatusMode(p, oamsearch);
                }
            }
            break;
        case vblank:
            if (p->ticks == 456) {
                p->ticks = 0;
                p->ram[LY] += 1;
                if (p->ram[LY] == 153) {
                    p->ram[LY] = 0;
                    // updatescreen();
                    setLcdStatusMode(p, oamsearch);
                }
            }
            break;
        case oamsearch:
            // collect sprite data
            // SCAN oam (object attribute memory)
            // from 0xfe00 to 0xfe9f to mix sprit pixels in the current line later
            if (p->ticks == 20) {
                p->pixels_drawn_on_current_line = 0;
                yDraw = p->ram[SCROLL_Y] + p->ram[LY];
                tileLine = p->ram[LY] % 8;
                tileMapRowAddr = (0x9800 + (yDraw/8) * 32);
                // this.fetcher.initForLine(tileMapRowAddr, tileLine);
                setLcdStatusMode(p, pixeltransfer);
            }
            break;
        case pixeltransfer:
            /* this.fetcher.tick(); */
            /* if (this.fetcher.fifo.length() < 8) { */
            /*     return; */
            /* } */
            /* const pixel = this.fetcher.fifo.deque(); */
            /* this.frameBuffer[(this.ly * LCD_SIZE_X) + this.pixelDrawnsOnCurrentLine] = pixel; */
            p->pixels_drawn_on_current_line += 1;
            if (p->pixels_drawn_on_current_line == 160) {
                setLcdStatusMode(p, hblank);
            }
            break;
    }
}
