#ifndef UTILS_H
#define UTILS_H

#include <errno.h>
#include <libopencm3/stm32/usart.h>
#include <stdio.h>

int _write(int file, char *ptr, int len);

#endif // UTILS_H
