#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "cpu.h"
#include "ppu.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define SCREEN_SURFACE_WIDTH 160
#define SCREEN_SURFACE_HEIGHT 144

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
    ppu p;
    int i;
    uint8_t* ram = (uint8_t*)malloc(sizeof(uint8_t)* RAM_SIZE);
    uint8_t* screen_surface =  (uint8_t*) malloc(sizeof(uint8_t) * SCREEN_SURFACE_WIDTH * SCREEN_SURFACE_HEIGHT);
    c.ram = ram;
    p.ram = ram;
    p.screen_surface = screen_surface;

    for(i = 0; i < RAM_SIZE; i++) {
        c.ram[i] = i % 0xff;
    }
    loadBootRom(&c);

    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL error : %s", SDL_GetError());
        exit(1);
    }
    window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("SDL error : %s", SDL_GetError());
        exit(1);
    }

    screenSurface = SDL_GetWindowSurface(window);
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
    SDL_UpdateWindowSurface(window);

    int error = 0;
    for(i = 0; i < 30000; i++) {
        tickCpu(&c);
        tickPpu(&p);
    }
    debugRegisters(&c);
    printf("%d\n", i);

    SDL_Delay(2000);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
