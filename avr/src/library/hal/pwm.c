/*
 * PWM generators
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#include <avr/io.h>
#include <inttypes.h>

#include "hal/pwm.h"

enum e_timer {
	PWM_TIMER_0 = 0,
	PWM_TIMER_1,
	PWM_TIMER_2
};

/*
 * Connect pin TOCCx to given timer output
 * Even pin numbers are connected to OCxB, odd to OCxA
 */
static void pwm_set_pin(uint8_t pin, enum e_timer timer)
{
	if (pin >= 7)
		return;

	//connect pin to compare output
	if (pin <= 3) {
		TOCPMSA0 &= ~0x03;
		TOCPMSA0 |= (timer & 0x03) << pin*2;
	} else {
		TOCPMSA1 &= ~0x03;
		TOCPMSA1 |= (timer & 0x03) << (pin-3)*2;
	}

	//set pin as compare output
	TOCPMCOE |= _BV(pin);
}

/*
 * Generate pwm from timer0 - 8 bit
 * frequency = F_OSC/(256*presc)
 *
 * Odd out number is connected to pwmA, even to pwmB due to MCU limitations
 * Out1 and 2 can't be both odd or even
 * Out is coresponding to TOCCx pin, output is not used when set to 0xff
 * Correcponding DDR must be set to output before calling
 */
void pwm_init_t0(enum e_presc presc, uint8_t out1, uint8_t out2)
{
	//no interrupts
	TIMSK0 = 0;

	//fast pwm, overflow at 0xff
	TCCR0A = 3;
	//enable counter and set prescaler
	TCCR0B = presc;

	if (out1 != 0xff) {
		if (out1 % 2)
			TCCR0A |= 2 << COM0A0;
		else
			TCCR0B |= 2 << COM0B0;
		pwm_set_pin(out1, PWM_TIMER_0);
	}
	if (out2 != 0xff) {
		if (out2 % 2)
			TCCR0A |= 2 << COM0A0;
		else
			TCCR0B |= 2 << COM0B0;
		pwm_set_pin(out2, PWM_TIMER_0);
	}
}

/*
 * Generate pwm from timer1 - 16 bit
 * frequency = F_OSC/((2^bits)*presc)
 *
 * Odd out number is connected to pwmA, even to pwmB due to MCU limitations
 * Out1 and 2 can't be both odd or even
 * Out is coresponding to TOCCx pin, output is not used when set to 0xff
 * Correcponding DDR must be set to output before calling
 */
void pwm_init_t1(enum e_presc presc, enum e_pwm_bits bits, uint8_t out1, uint8_t out2)
{
	//no interrupts
	TIMSK1 = 0;

	//fast pwm, 8,9 or 10 bits mode
	TCCR1A = bits;
	//enable counter and set prescaler
	TCCR1B = presc | 0x08;

	if (out1 != 0xff) {
		if (out1 % 2)
			TCCR1A |= 2 << COM1A0;
		else
			TCCR1B |= 2 << COM1B0;
		pwm_set_pin(out1, PWM_TIMER_1);
	}
	if (out2 != 0xff) {
		if (out2 % 2)
			TCCR1A |= 2 << COM1A0;
		else
			TCCR1B |= 2 << COM1B0;
		pwm_set_pin(out2, PWM_TIMER_1);
	}
}
