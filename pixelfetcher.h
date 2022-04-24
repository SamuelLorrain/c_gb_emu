#ifndef PIXEL_FETCHER_H
#define PIXEL_FETCHER_H
#include <stdint.h>
#include <stdbool.h>
#include "pixelFifo.h"

#define TILE_DATA_SIZE 8

typedef enum pixel_fetcher_state {
    read_tile_id,
    read_tile_0,
    read_tile_1,
    push_to_fifo
} pixel_fetcher_state;

typedef struct {
    uint8_t currentMapAddr;
    uint8_t currentLine;
    uint8_t currentTileLow;
    uint8_t currentTileHigh;
    uint8_t currentTileId;
    uint8_t tick;
    uint8_t tileIndex;
    uint8_t* ram;
    uint8_t* tileData;
    pixel_fifo* fifo;
    pixel_fetcher_state state;

} pixel_fetcher;

pixel_fetcher newPixelFetcher(uint8_t* ram, pixel_fifo* pfifo);
void deletePixelFetcher(pixel_fetcher* pf);

void initForLine(pixel_fetcher* pf, uint8_t mapAddr, uint8_t line);
void readTileForLine(pixel_fetcher* pf, bool bitPlane);
void pushToFifo(pixel_fetcher* pf);
void tickPixelFetcher(pixel_fetcher* pf);

#endif
