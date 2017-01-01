/*
 * Events timing
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#ifndef __TIMING_H_
#define __TIMING_H_

#include <inttypes.h>

/*
 * Call function every period interval n times
 *
 * Period is in base loop time - 10 ms
 * Returns 0 if suceeded, 1 if all callbacks are already used
 */
extern uint8_t timing_callback_add(void (*function)(void), uint8_t period,
					uint8_t n);

/*
 * Delete function from timing is defined
 */
extern void timing_callback_cancel(void (*function)(void));

/*
 * Runs scheduled functions
 *
 * Call periodically
 */
extern void timing_tick(void);

#endif
