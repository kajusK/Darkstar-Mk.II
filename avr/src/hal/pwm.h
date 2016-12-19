/*
 * PWM generators
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#ifndef __PWM_H_
#define __PWM_H_

#include <inttypes.h>

//set pwm duty on timer0, output A
#define pwm_t0_setA(duty) { OCR0A = duty; }
#define pwm_t0_setB(duty) { OCR0B = duty; }

#define pwm_t1_setA(duty) { OCR1A = duty; }
#define pwm_t1_setB(duty) { OCR1B = duty; }

enum e_presc {
	PWM_PRESC_1 = 1,
	PWM_PRESC_8,
	PWM_PRESC_64,
	PWM_PRESC_256,
	PWM_PRESC_1024,
};

enum e_pwm_bits {
	PWM_8_BITS = 1,
	PWM_9_BITS,
	PWM_10_BITS,
};

/*
 * Generate pwm from timer0 - 8 bit
 * frequency = F_OSC/(256*presc)
 *
 * Odd out number is connected to pwmA, even to pwmB due to MCU limitations
 * Out1 and 2 can't be both odd or even
 * Out is coresponding to TOCCx pin, output is not used when set to 0xff
 * Correcponding DDR must be set to output before calling
 */
extern void pwm_init_t0(enum e_presc presc, uint8_t out1, uint8_t out2);

/*
 * Generate pwm from timer1 - 16 bit
 * frequency = F_OSC/((2^bits)*presc)
 *
 * Odd out number is connected to pwmA, even to pwmB due to MCU limitations
 * Out1 and 2 can't be both odd or even
 * Out is coresponding to TOCCx pin, output is not used when set to 0xff
 * Correcponding DDR must be set to output before calling
 */
extern void pwm_init_t1(enum e_presc presc, enum e_pwm_bits bits, uint8_t out1,
			uint8_t out2);

#endif
