/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <errno.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/dwt.h>
#include <stdio.h>
#include <unistd.h>

#define WAIT_NUMBER                                                            \
	1600000 // for a relevant wait time amount write 800000 or more

int _write(int file, char *ptr, int len)
{
	int i;

	if (file == 1) {
		for (i = 0; i < len; i++)
			usart_send_blocking(USART1, ptr[i]);
		return i;
	}

	errno = EIO;
	return -1;
}

static void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

	/* Enable LED clock. */
	rcc_periph_clock_enable(RCC_INTERNAL_LED);

	/* Set internal LED */
	gpio_set_mode(INTERNAL_LED_PORT, GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, INTERNAL_LED);

	

	/* Enable USART clock. */
	rcc_periph_clock_enable(RCC_GPIOA); // Needed when BOOT0 is 0
	rcc_periph_clock_enable(RCC_USART1);

}

/*
 * Usart parameters
 */
#define USART_BAUDRATE 115200
#define USART_DATABITS 9
#define USART_STOPBITS USART_STOPBITS_1
#define USART_MODE USART_MODE_TX
#define USART_PARITY USART_PARITY_EVEN
#define USART_FLOWCONTROL USART_FLOWCONTROL_NONE

static void usart_setup(void)
{
	/* Setup GPIO pin GPIO_USART1_TX. */
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

	/* Setup USART PARAMETERS */
	usart_set_baudrate(USART1, USART_BAUDRATE);
	usart_set_databits(USART1, USART_DATABITS);
	usart_set_stopbits(USART1, USART_STOPBITS);
	usart_set_mode(USART1, USART_MODE);
	usart_set_parity(USART1, USART_PARITY);
	usart_set_flow_control(USART1, USART_FLOWCONTROL);

	/* Enable USART */
	usart_enable(USART1);
}


int main(void)
{

	SCB_VTOR = (uint32_t)0x08000000;
	
	clock_setup();
	usart_setup();

	dwt_enable_cycle_counter();

	uint32_t last_measured_cycle = 0;
	uint32_t current_measured_cycle = 0;

	// Blinks led and prints half-period time
	while (1) {
		last_measured_cycle = dwt_read_cycle_counter();

		gpio_toggle(INTERNAL_LED_PORT, INTERNAL_LED); /* LED on/off */

		for (int i = 0; i < WAIT_NUMBER; i++) /* Wait a bit. */
			__asm__("nop");

		current_measured_cycle = dwt_read_cycle_counter();
		printf("%u us\n", (current_measured_cycle - last_measured_cycle) / 72);
	}

	return 0;
}
