#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "cpu.h"
#include "ppu.h"
#include "ram.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define SCREEN_SURFACE_WIDTH 160
#define SCREEN_SURFACE_HEIGHT 144

#define SdlInitMacro() \
    if(SDL_Init(SDL_INIT_VIDEO) < 0) { \
        printf("SDL error : %s", SDL_GetError()); \
        exit(1); \
    } \
    window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); \
    if (window == NULL) { \
        printf("SDL error : %s", SDL_GetError()); \
        exit(1); \
    } \
    screenSurface = SDL_GetWindowSurface(window); \
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF)); \
    SDL_UpdateWindowSurface(window)

#define SdlQuitMacro() \
    SDL_DestroyWindow(window); \
    SDL_Quit() \

void loadBootRom(cpu* c) {
    FILE* file = fopen("DMG_ROM.bin", "rb");
    fseek(file, 0, SEEK_END);
    long fileLength = ftell(file);
    rewind(file);
    fread(c->ram, 1, fileLength, file);
}

int main() {
    long count = 0;
    uint8_t* ram = newRam(RAM_SIZE);
    uint8_t* screen_surface = newRam(SCREEN_SURFACE_WIDTH * SCREEN_SURFACE_HEIGHT);
    pixel_fifo pfifo = newFifo();
    pixel_fetcher pfetcher = newPixelFetcher(ram, &pfifo);
    ppu p = newPpu(&pfetcher);
    cpu c = newCpu();
    int i;
    c.ram = ram;
    p.ram = ram;
    p.screen_surface = screen_surface;

    loadBootRom(&c);
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    SdlInitMacro();

    for(i = 0; c.reg.pc < 0x64; i++) {
        tickCpu(&c);
        tickPpu(&p);
    }

    debugRegisters(&c);
    debugRam(ram, 0xff44, 5);

    printf("ticks : %d\n", i);

    /* SDL_Delay(2000); */
    SdlQuitMacro();
    free(ram);
    free(screen_surface);
    deleteFifo(&pfifo);
    deletePixelFetcher(&pfetcher);
    return 0;
}
