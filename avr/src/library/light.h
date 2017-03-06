/*
 * Light control
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#ifndef __LIGHT_H_
#define __LIGHT_H_

#include <inttypes.h>

enum e_led {
	LED_SPOT = 0,
	LED_FLOOD,
	LED_WHITE,
	LED_RED,
};

enum light_mode {
	//output is set to given level, light output decreases with voltage
	MODE_NORMAL = 0,
	//output level is adjusted for constant light during battery discharge
	MODE_CONSTANT,
	//output level is modified by external light sensor
	MODE_AUTO,
	//has no effect when setting light output, used only when getting
	//currently used mode (when LED is running blink task)
	MODE_BLINK,
	//led is off, setting led level to 0 is equivalent to this mode
	MODE_OFF,
};

struct s_limits {
	/* light limited to minimal level */
	uint8_t undervoltage : 1;
	uint8_t overheating : 1;
	/* main leds disabled to prevent permanent damage */
	uint8_t voltage_shutdown : 1;
	uint8_t heat_shutdown : 1;
};

/*
 * Get currently active limits applied to light output
 */
extern struct s_limits light_limits(void);

/*
 * Update LED output based on mode, light limits,...
 * Should be called periodically
 */
extern void light_update(void);

/*
 * Turn off all LEDs
 */
extern void light_off(void);

/*
 * Set light output to given level if possible, for e.g. small leds, only
 * off and on states are possible (false/true)
 *
 * Output light is adjusted to be linear to human eye (steps on low
 * values are small, higher values make big steps)
 */
extern void light_set(enum e_led led, uint8_t level, enum light_mode mode);

/*
 * Set light output to given level if possible, for e.g. small leds, only
 * off and on states are possible (false/true)
 *
 * No linearization is applied
 */
extern void light_set_simple(enum e_led led, uint8_t level,
				enum light_mode mode);

/*
 * Get LED status
 */
extern enum light_mode light_get(enum e_led led);

/*
 * Limit all outputs to this value
 */
extern void light_set_limit(uint8_t limit);

/*
 * Blink led
 * times are in tenths of ms
 *
 * Only one led can be blinking at the same time, if there's blink task running
 * when called, it will be replaced by the new one
 *
 * Led on level can be set by turning LED on (light_set()), if LED is off when
 * called, level will be set to default value
 *
 * After blinking is finished, output is set to mode used before calling
 */
extern void light_blink(enum e_led led, uint8_t on_time, uint8_t period,
			uint8_t times);

/*
 * True if there's no blink mode running
 */
extern uint8_t light_get_blink_finished(void);

#endif
