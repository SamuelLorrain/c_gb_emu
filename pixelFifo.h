#ifndef PIXEL_FIFO
#define PIXEL_FIFO
#include <stdint.h>

#define PIXEL_FIFO_SIZE 32

typedef struct {
    uint8_t* container;
    uint8_t start;
    uint8_t finish;
    uint8_t len;
} pixel_fifo;

pixel_fifo newFifo();
void deleteFifo(pixel_fifo* pfifo);
void enqueue(pixel_fifo* pfifo, uint8_t v);
uint8_t dequeue(pixel_fifo* pfifo);
void clearFifo(pixel_fifo* pfifo);


#endif
