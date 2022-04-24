#ifndef PPU_H
#define PPU_H
#include <stdint.h>
#include <stdbool.h>

#define LCD_CONTROL 0xff40
#define LCD_STATUS 0xff41
#define SCROLL_Y 0xff42
#define SCROLL_X 0xff43
#define LY 0xff44
#define LYC 0xff45

typedef enum {
    hblank,
    vblank,
    oamsearch,
    pixeltransfer,
} ppu_state_mode;

typedef struct {
    bool onOff;
    uint8_t* ram;
    uint8_t* screen_surface;
    uint32_t ticks;
    uint8_t pixels_drawn_on_current_line;
} ppu;

void tickPpu(ppu* p);
void setLcdStatusMode(ppu* p, ppu_state_mode s);

ppu newPpu();

#endif
