#ifndef __DMA_H
#define __DMA_H

#include <errno.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/usart.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define DMA_BUFFER_SIZE 256

void dma_update();
void write_dma(char *p, int len);

#endif // __DMA_H
