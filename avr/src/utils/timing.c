/*
 * Events timing
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#include <inttypes.h>
#include <stdio.h>

#define CALLBACK_NUM 2

struct callback {
	void (*function)(void);
	uint8_t period;
	uint8_t data;
	uint8_t times;
	uint8_t next_time;
};

static struct callback callbacks[CALLBACK_NUM];
static uint8_t timer;

/*
 * Call function every period interval n times
 *
 * Period is in base loop time - 10 ms
 * Returns 0 if suceeded, 1 if all callbacks are already used
 */
uint8_t timing_callback_add(void (*function)(void), uint8_t period, uint8_t n)
{
	uint8_t i;

	for (i = 0; i < CALLBACK_NUM; i++) {
		if (callbacks[i].function == NULL) {
			callbacks[i].function = function;
			callbacks[i].period = period;
			callbacks[i].times = n;
			callbacks[i].next_time = timer + period;
			return 0;
		}
	}
	return 1;
}

/*
 * Delete function from timing is defined
 */
void timing_callback_cancel(void (*function)(void))
{
	uint8_t i;
	for (i = 0; i < CALLBACK_NUM; i++) {
		if (callbacks[i].function == function) {
			callbacks[i].function = NULL;
			return;
		}
	}
}

/*
 * Runs scheduled functions
 *
 * Call periodically
 */
void timing_tick(void)
{
	uint8_t i;

	timer++;

	for (i = 0; i < CALLBACK_NUM; i++) {
		if (callbacks[i].function == NULL)
			continue;

		if (callbacks[i].next_time != timer)
			continue;

		(*callbacks[i].function)();
		callbacks[i].times--;
		callbacks[i].next_time += callbacks[i].period;

		if (callbacks[i].times == 0)
			callbacks[i].function = NULL;
	}
}
