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
#define TIMER_MAX (F_CPU/1000)

static volatile uint16_t time;

ISR(TIMER2_COMPA_vect)
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

	//execute interrupt when OCR2A matched
	//attiny does not set overflow flag in this config...
	TIMSK2 |= _BV(OCIE2A);
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
