/*
 * LEDs control
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#include <avr/io.h>
#include <inttypes.h>

#include "config.h"
#include "hal/pwm.h"

#if (LED1_PWM_NUM % 2 == LED2_PWM_NUM % 2)
	#error "LEDx_PWM_NUM can't be both even (or odd) due to avr internals"
#endif

/*
 * Setup ports, pwm, etc...
 */
void leds_init(void)
{
	DDR(LED1_PWM_PORT) |= _BV(LED1_PWM_PIN);
	DDR(LED2_PWM_PORT) |= _BV(LED2_PWM_PIN);
	pwm_init_t0(PWM_PRESC_1, LED1_PWM_NUM, LED2_PWM_NUM);
	pwm_t0_setA(0);
	pwm_t0_setB(0);

	PORT(DRIVER_ENABLE_PORT) |= _BV(DRIVER_ENABLE_PIN);
	DDR(DRIVER_ENABLE_PORT) |= _BV(DRIVER_ENABLE_PIN);

	PORT(LED_SM1_PORT) |= _BV(LED_SM1_PIN);
	PORT(LED_SM2_PORT) |= _BV(LED_SM2_PIN);
	DDR(LED_SM1_PORT) |= _BV(LED_SM1_PIN);
	DDR(LED_SM2_PORT) |= _BV(LED_SM2_PIN);

	// if small leds are on port with high sink option, enable it
	#if (MCU == attiny841)
		#if ((LED_SM1_PORT == 'A' && LED_SM1_PIN == 5) ||\
		    (LED_SM2_PORT == 'A' && LED_SM2_PIN == 5))
			PHDE |= _BV(PHDEA0);
		#endif

		#if ((LED_SM1_PORT == 'A' && LED_SM1_PIN == 7) ||\
		    (LED_SM2_PORT == 'A' && LED_SM2_PIN == 7))
			PHDE |= _BV(PHDEA1);
		#endif
	#endif
}

/*
 * Turn on led drivers, pwm output is not modified
 */
void leds_enable(void)
{
	DDR(LED1_PWM_PORT) |= _BV(LED1_PWM_PIN);
	DDR(LED2_PWM_PORT) |= _BV(LED2_PWM_PIN);
	PORT(DRIVER_ENABLE_PORT) &= ~_BV(DRIVER_ENABLE_PIN);
}

/*
 * Turn off led drivers
 */
void leds_disable(void)
{
	PORT(DRIVER_ENABLE_PORT) |= _BV(DRIVER_ENABLE_PIN);
	DDR(LED1_PWM_PORT) &= ~_BV(LED1_PWM_PIN);
	DDR(LED2_PWM_PORT) &= ~_BV(LED2_PWM_PIN);
}
