/*
 * LEDs control
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#ifndef __LEDS_H_
#define __LEDS_H_

#include "hal/pwm.h"
#include "config.h"

/*
 * Setup ports, pwm, etc...
 */
extern void leds_init(void);

/*
 * Turn on led drivers, pwm output is not modified
 */
extern void leds_enable(void);

/*
 * Turn off led drivers
 */
extern void leds_disable(void);

/*
 * Set led current, 0-255
 */
#if (LED1_PWM_NUM % 2 == 0)
	#define led1_set(duty) { pwm_t0_setB(duty); }
	#define led1_off() { pwm_t0_setB(0); }
	#define led2_set(duty) { pwm_t0_setA(duty); }
	#define led2_off() { pwm_t0_setA(0); }
#else
	#define led1_set(duty) { pwm_t0_setA(duty); }
	#define led1_off() { pwm_t0_setA(0); }
	#define led2_set(duty) { pwm_t0_setB(duty); }
	#define led2_off() { pwm_t0_setB(0); }
#endif

/*
 * Small leds control
 */
#define led_sm1_on() { PORT(LED_SM1_PORT) |= _BV(LED_SM1_PIN); }
#define led_sm1_off() { PORT(LED_SM1_PORT) &= ~_BV(LED_SM1_PIN); }
#define led_sm2_on() { PORT(LED_SM2_PORT) |= _BV(LED_SM2_PIN); }
#define led_sm2_off() { PORT(LED_SM2_PORT) &= ~_BV(LED_SM2_PIN); }

#endif
