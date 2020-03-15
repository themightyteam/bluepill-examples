#include "cron.h"

static volatile uint32_t millisecs_from_start = 0;

/*
 * @brief interrupt callback
 */
void sys_tick_handler(void)
{
	millisecs_from_start++;
}

/*
 * @brief get the number of milliseconds from start
 */
uint32_t get_millisecs_from_start()
{
	return millisecs_from_start;
}
