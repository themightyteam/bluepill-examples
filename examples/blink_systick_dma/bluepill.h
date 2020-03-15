#ifndef __BLUEPILL_H
#define __BLUEPILL_H

#include "cron.h"
#include "dma.h"
#include <errno.h>
#include <libopencm3/cm3/dwt.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <stdio.h>
#include <unistd.h>

void bluepill_setup(void);
void bluepill_update(void);

#endif // __BLUEPILL_H