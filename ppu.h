#ifndef PPU_H
#define PPU_H
#include <stdint.h>

#define LCD_CONTROL 0xff40
#define LCD_STATUS 0xff41
#define LY 0xff44
#define LYC 0xff45
#define isLcdOnMacro(p) (p->ram[LCD_CONTROL] & 0b10000000) == 0b1000000

typedef enum {
    hblank,
    vblank,
    oamsearch,
    pixeltransfer,
} ppu_state_mode;

typedef struct {
    uint8_t* ram;
    uint8_t* screen_surface;
} ppu;

void tickPpu(ppu* p);
void setLcdStatusMode(ppu* p, ppu_state_mode s);

#endif
