#include "pixelfetcher.h"
#include <stdlib.h>

pixel_fetcher newPixelFetcher(uint8_t* ram, pixel_fifo* pfifo) {
    pixel_fetcher pf;
    pf.currentMapAddr = 0;
    pf.currentLine = 0;
    pf.currentTileLow = 0;
    pf.currentTileHigh = 0;
    pf.currentTileId = 0;
    pf.tick = 0;
    pf.state = read_tile_id;
    pf.tileIndex = 0;
    pf.ram = ram;
    pf.fifo = pfifo;
    pf.tileData = (uint8_t*) malloc(sizeof(uint8_t)*TILE_DATA_SIZE);
    return pf;
}

void deletePixelFetcher(pixel_fetcher* pf) {
    free(pf->tileData);
}

void initForLine(pixel_fetcher* pf, uint8_t mapAddr, uint8_t line) {
    pf->tileIndex = 0;
    pf->currentMapAddr = mapAddr;
    pf->currentLine = line;
    pf->state = read_tile_id;
    clearFifo(pf->fifo);
}

void readTileForLine(pixel_fetcher* pf, bool bitPlane) {
    uint16_t offset = 0x8000 + (pf->currentTileId * 16);
    uint16_t addr = offset + (pf->currentLine * 2);
    uint8_t pixelData = pf->ram[addr + bitPlane];
    for(int i = 0; i < 8; i++) {
        if (bitPlane) {
            pf->tileData[i] = (pixelData >> i) & 0b1;
        } else {
            pf->tileData[i] |= (((pixelData >> i) & 0b1) << 1);
        }
    }
}

void pushToFifo(pixel_fetcher* pf) {
    for(int i = 0; i >= 0; i--) {
        enqueue(pf->fifo, pf->tileData[i]);
    }
}

void tickPixelFetcher(pixel_fetcher* pf) {
    pf->tick += 1;
    if (pf->tick < 2) {
        return;
    }
    pf->tick = 0;

    switch(pf->state) {
        case read_tile_id:
            pf->currentTileId = pf->ram[
                pf->currentMapAddr + pf->tileIndex
            ];
            pf->state = read_tile_0;
            break;
        case read_tile_0:
            readTileForLine(pf, 0);
            pf->state = read_tile_1;
            break;
        case read_tile_1:
            readTileForLine(pf, 1);
            pf->state = push_to_fifo;
            break;
        case push_to_fifo:
            if (pf->fifo->len <= 8) {
                pushToFifo(pf);
                pf->state = read_tile_id;
            }
            break;
    }
}
