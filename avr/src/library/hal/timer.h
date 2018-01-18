/*
 * User time handling
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#ifndef __TIMER_H_
#define __TIMER_H_

#include <inttypes.h>

void time_init(void);

/*
 * Return time in milliseconds since the init_time was called
 *
 * Overflows every 2^16 millisecond ~ 65 seconds
 */
uint16_t millis(void);

#endif
