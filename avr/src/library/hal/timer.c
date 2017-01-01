/*
 * User time handling
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#include <avr/io.h>
#include <util/atomic.h>
#include <avr/interrupt.h>

//timer2 overflow number
#define TIMER_MAX (F_CPU*0.001)

static volatile uint16_t time;

ISR(TIMER2_OVF_vect)
{
	time++;
}

void time_init(void)
{
	TCNT2 = 0;
	time = 0;

	OCR2A = TIMER_MAX;
	//no prescaler, clear on OCR2A match
	TCCR2A = 0;
	TCCR2B = _BV(WGM22) | _BV(CS20);

	TIMSK2 |= _BV(TOIE2);
}

/*
 * Return time in milliseconds since the init_time was called
 *
 * Overflows every 2^16 millisecond ~ 65 seconds
 */
uint16_t millis(void)
{
	uint16_t int_time;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		int_time = time;
	}

	return int_time;
}

/*
 * Get difference between two times in ms with single overflow in mind,
 * the difference must be shorter than 2^16 ms ~ 65 s
 *
 * t2 timestamp has to be taken after t1
 */
uint16_t time_diff(uint16_t t1, uint16_t t2)
{
	if (t1 < t2)
		return t2 - t1;

	return (65536 - t1) + t2;
}
