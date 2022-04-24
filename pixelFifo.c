#include "pixelFifo.h"
#include <stdlib.h>
#include <stdint.h>

pixel_fifo newFifo() {
    pixel_fifo pfifo;
    pfifo.container = (uint8_t*)malloc(sizeof(uint8_t)*PIXEL_FIFO_SIZE);
    pfifo.start = PIXEL_FIFO_SIZE;
    pfifo.finish = PIXEL_FIFO_SIZE-1;
    pfifo.len = 0;
    return pfifo;
}

void deleteFifo(pixel_fifo* pfifo) {
    free(pfifo->container);
}

void enqueue(pixel_fifo* pfifo, uint8_t v) {
    if (pfifo->start) {
        pfifo->start -= 1;
    } else {
        pfifo->start = PIXEL_FIFO_SIZE - 1;
    }
    pfifo->container[pfifo->start] = v;
    pfifo->len += 1;
}

uint8_t dequeue(pixel_fifo* pfifo) {
    uint8_t v = pfifo->container[pfifo->finish];
    if(pfifo->finish > 0) {
        pfifo->finish -= 1;
    } else {
        pfifo->finish = PIXEL_FIFO_SIZE-1;
    }
    pfifo->len -= 1;
    return v;
}

void clearFifo(pixel_fifo* pfifo) {
    pfifo->start = PIXEL_FIFO_SIZE;
    pfifo->finish = PIXEL_FIFO_SIZE-1;
    pfifo->len = 0;
}

