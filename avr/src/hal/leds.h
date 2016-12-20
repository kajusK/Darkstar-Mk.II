/*
 * LEDs control
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#ifndef __LEDS_H_
#define __LEDS_H_

#include <avr/io.h>
#include <inttypes.h>
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
 * Setup current used when dimming enabled
 */
extern void leds_dim_level(uint8_t level);

/*
 * Set led 1 output level
 *
 * Current in 0-255
 * If dimming_enable, current is set to dim level (leds_dim_level()) and
 * 0-255 is used for additional pwm dimming
 */
extern void led1_set(uint8_t brightness, uint8_t dimming_enable);

/*
 * Set led 2 output level
 *
 * Current in 0-255
 * If dimming_enable, current is set to dim level (leds_dim_level()) and
 * 0-255 is used for additional pwm dimming
 */
extern void led2_set(uint8_t brightness, uint8_t dimming_enable);

/*
 * Small leds control
 */
#define led_sm1_on() { PORT(LED_SM1_PORT) |= _BV(LED_SM1_PIN); }
#define led_sm1_off() { PORT(LED_SM1_PORT) &= ~_BV(LED_SM1_PIN); }
#define led_sm2_on() { PORT(LED_SM2_PORT) |= _BV(LED_SM2_PIN); }
#define led_sm2_off() { PORT(LED_SM2_PORT) &= ~_BV(LED_SM2_PIN); }

#endif
