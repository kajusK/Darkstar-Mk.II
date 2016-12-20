/*
 * User time handling
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#ifndef __ADC_H_
#define __ADC_H_

#include <inttypes.h>

void init_time(void);

/*
 * Return time in milliseconds since the init_time was called
 *
 * Overflows every 2^16 millisecond ~ 65 seconds
 */
uint16_t millis(void);

/*
 * Get difference between two times in ms with single overflow in mind,
 * the difference must be shorter than 2^16 ms ~ 65 s
 *
 * t2 timestamp has to be taken after t1
 */
extern uint16_t diff_time(uint16_t t1, uint16_t t2);

#endif
