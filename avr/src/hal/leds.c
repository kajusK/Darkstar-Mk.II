/*
 * LEDs control
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>

#include "config.h"
#include "hal/pwm.h"

#if (LED1_PWM_NUM % 2 == LED2_PWM_NUM % 2)
	#error "LEDx_PWM_NUM can't be both even (or odd) due to avr internals"
#endif

static uint8_t dim_level = DEFAULT_DIM_CURRENT;

/*
 * Set led current, 0-255
 */
#if (LED1_PWM_NUM % 2 == 0)
	#define led1_duty(duty) { pwm_t0_setB(duty); }
	#define led2_duty(duty) { pwm_t0_setA(duty); }
#else
	#define led1_duty(duty) { pwm_t0_setA(duty); }
	#define led2_duty(duty) { pwm_t0_setB(duty); }
#endif

#define led1_dim_enable() { TIMSK1 |= _BV(OCIE1A); }
#define led1_dim_disable() { TIMSK1 &= ~_BV(OCIE1A); \
			     DDR(LED2_DIM_PORT) &= ~_BV(LED2_DIM_PIN); }
#define led2_dim_enable() { TIMSK1 |= _BV(OCIE1B); }
#define led2_dim_disable() { TIMSK1 &= ~_BV(OCIE1B); \
			     DDR(LED2_DIM_PORT) &= ~_BV(LED2_DIM_PIN); }


/*
 * Led dimming
 */
ISR(TIMER1_COMPA_vect)
{
	DDR(LED1_DIM_PORT) |= _BV(LED1_DIM_PIN);
}

ISR(TIMER1_COMPB_vect)
{
	DDR(LED2_DIM_PORT) |= _BV(LED2_DIM_PIN);
}


ISR(TIMER1_OVF_vect)
{
	DDR(LED2_DIM_PORT) &= ~_BV(LED2_DIM_PIN);
	DDR(LED2_DIM_PORT) &= ~_BV(LED2_DIM_PIN);
}

/*
 * Setup ports, pwm, etc...
 */
void leds_init(void)
{
	//pwm for setting led current
	DDR(LED1_PWM_PORT) |= _BV(LED1_PWM_PIN);
	DDR(LED2_PWM_PORT) |= _BV(LED2_PWM_PIN);
	pwm_init_t0(PWM_PRESC_1, LED1_PWM_NUM, LED2_PWM_NUM);
	pwm_t0_setA(0);
	pwm_t0_setB(0);

	//led driver power control
	PORT(DRIVER_ENABLE_PORT) |= _BV(DRIVER_ENABLE_PIN);
	DDR(DRIVER_ENABLE_PORT) |= _BV(DRIVER_ENABLE_PIN);

	//additional led pwm dimming
	pwm_init_t1(PWM_PRESC_8, PWM_8_BITS, 0xff, 0xff);
	pwm_t1_setA(0xff);
	pwm_t1_setB(0xff);
	PORT(LED1_DIM_PORT) &= ~_BV(LED1_DIM_PIN);
	PORT(LED2_DIM_PORT) &= ~_BV(LED2_DIM_PIN);
	TIMSK1 |= _BV(TOIE1);

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

/*
 * Setup current used when dimming enabled
 */
void leds_dim_level(uint8_t level)
{
	dim_level = level;
}

/*
 * Set led 1 output level
 *
 * Current in 0-255
 * If dimming_enable, current is set to dim level (leds_dim_level()) and
 * 0-255 is used for additional pwm dimming
 */
void led1_set(uint8_t brightness, uint8_t dimming_enable)
{
	if (!dimming_enable) {
		led1_dim_disable();
		led1_duty(brightness);
		return;
	}

	led1_duty(dim_level);
	pwm_t1_setA(brightness);
	led1_dim_enable();
}

/*
 * Set led 2 output level
 *
 * Current in 0-255
 * If dimming_enable, current is set to dim level (leds_dim_level()) and
 * 0-255 is used for additional pwm dimming
 */
void led2_set(uint8_t brightness, uint8_t dimming_enable)
{
	if (!dimming_enable) {
		led2_dim_disable();
		led2_duty(brightness);
		return;
	}

	led2_duty(dim_level);
	pwm_t1_setB(brightness);
	led2_dim_enable();
}
