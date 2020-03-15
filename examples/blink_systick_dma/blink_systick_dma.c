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

#include "bluepill.h"

int main(void)
{

	bluepill_setup();

	uint32_t last_millisecond = 0;
	uint32_t current_millisecond = 0;

	while (1) {

		bluepill_update();

		current_millisecond = get_millisecs_from_start();

		if (current_millisecond - last_millisecond >= 500) {

			last_millisecond = current_millisecond;

			////// CODE HERE ///////
			gpio_toggle(INTERNAL_LED_PORT,
				    INTERNAL_LED); /* LED on/off */
			printf("%lu ms\n", last_millisecond);
			////////////////////////
		}
	}

	return 0;
}
