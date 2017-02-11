/*
 * Light control
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#include <inttypes.h>

#include "config.h"
#include "hal/adc.h"
#include "hal/leds.h"
#include "system.h"
#include "light.h"

//used during linear calculation, should be equal to max supply voltage (in mV)
//but the real value is not that important
#define VOLTAGE_MAX 4200

#define LEDS_COUNT 4

struct led_setup {
	uint8_t level;
	enum light_mode mode;
};

struct s_blink {
	uint8_t timer;
	uint8_t period;
	uint8_t on_time;
	uint8_t count;
	enum light_mode prev_mode;
};

static struct led_setup leds_setup[LEDS_COUNT];
static struct s_blink blink;

/*
 * Calculate level for constant mode
 */
static inline uint8_t mode_constant(uint8_t level)
{
	uint16_t voltage = system_voltage();

	//*10 and /10 used to avoid integer overflow
	uint16_t res = ((((uint16_t) VOLTAGE_MAX*10)/voltage)*level)/10;
	return res > 255 ? 255 : res;
}

/*
 * Calculate level for automatic mode
 */
static uint8_t mode_auto(uint8_t level)
{
	//TODO
	//limit to level+X and level - X
	return level;
}

static void led_update(enum e_led led, uint8_t level, uint8_t max)
{
	uint8_t dimming_enable = 0;

	if (led >= LEDS_COUNT)
		return;

	//apply limits for leds except small ones
	if (level > max && led != LED_RED && led != LED_WHITE)
		level = max;

	if (level < DEFAULT_DIM_CURRENT && level != 0){
		dimming_enable = 1;
		level *= 255/DEFAULT_DIM_CURRENT;
	}

	switch (led) {
	case LED_SPOT:
		led1_set(level, dimming_enable);
		break;
	case LED_FLOOD:
		led2_set(level, dimming_enable);
		break;
	case LED_RED:
		if (level) {
			led_sm1_on();
		} else {
			led_sm1_off();
		}
		break;
	case LED_WHITE:
		if (level) {
			led_sm2_on();
		} else {
			led_sm2_off();
		}
		break;
	}
}

/*
 * Get currently active limits applied to light output
 */
struct s_limits light_limits(void)
{
	static struct s_limits limits = { 0, 0, 0, 0 };
	uint8_t temp = adc_core_temp();
	uint16_t voltage = system_voltage();

	if (temp >= TEMP_MAX)
		limits.overheating = 1;
	else if (temp <= TEMP_MAX - TEMP_HYSTERSIS)
		limits.overheating = 0;

	if (temp >= TEMP_SHUTDOWN)
		limits.heat_shutdown = 1;
	else if (temp <= TEMP_SHUTDOWN - TEMP_HYSTERSIS)
		limits.heat_shutdown = 0;

	if (voltage <= VOLTAGE_MIN)
		limits.undervoltage = 1;
	else if (voltage >= VOLTAGE_MIN + VOLTAGE_HYSTERSIS)
		limits.undervoltage = 0;

	if (voltage <= VOLTAGE_SHUTDOWN)
		limits.voltage_shutdown = 1;
	else if (voltage >= VOLTAGE_SHUTDOWN + VOLTAGE_HYSTERSIS)
		limits.voltage_shutdown = 0;

	return limits;
}

/*
 * Update LED output based on mode, light limits,...
 * Should be called periodically
 */
void light_update(void)
{
	uint8_t i;
	uint8_t pwm_max = MAX_PWM;
	struct s_limits limits = light_limits();

	/*
	 * Apply limits
	 */
	/* overheating limit */
	if (limits.overheating)
		pwm_max = pwm_max < PWM_OVERHEAT_MAX ? pwm_max : PWM_OVERHEAT_MAX;
	/* undervoltage limit */
	if (limits.undervoltage)
		pwm_max = pwm_max < PWM_UNDERV ? pwm_max : PWM_UNDERV;

	if (limits.heat_shutdown || limits.voltage_shutdown) {
		leds_disable();
		pwm_max = 0;
	} else {
		if (leds_setup[LED_SPOT].level == 0 &&
				leds_setup[LED_FLOOD].level == 0)
			leds_disable();
		else
			leds_enable();
	}

	/*
	 * Update led status
	 */
	for (i = 0; i < LEDS_COUNT; i++) {
		switch (leds_setup[i].mode) {
		case MODE_NORMAL:
			led_update(i, leds_setup[i].level, pwm_max);
			break;
		case MODE_CONSTANT:
			led_update(i, mode_constant(leds_setup[i].level),
				   pwm_max);
			break;
		case MODE_AUTO:
			led_update(i, mode_auto(leds_setup[i].level), pwm_max);
			break;
		case MODE_BLINK:
			if (blink.count == 0) {
				leds_setup[i].mode = blink.prev_mode;
				break;
			}
			if (blink.timer == 0)
				led_update(i, leds_setup[i].level, pwm_max);
			else if (blink.timer == blink.on_time)
				led_update(i, 0, pwm_max);
			break;
		case MODE_OFF:
			led_update(i, 0, pwm_max);
			break;
		default:
			break;
		}
	}

	/*
	 * Blinking counters
	 */
	if (blink.count != 0) {
		blink.timer++;

		if (blink.timer == blink.period) {
			blink.timer = 0;
			blink.count--;
		}
	}
}

/*
 * Turn off all LEDs
 */
void light_off(void)
{
	int i;
	for (i = 0; i < LEDS_COUNT; i++)
		leds_setup[i].level = 0;
}

/*
 * Set light output to given level if possible, for e.g. small leds, only
 * off and on states are possible (false/true)
 *
 * Output light is adjusted to be linear to human eye (steps on low
 * values are small, higher values make big steps)
 *
 * https://en.wikipedia.org/wiki/Stevens'_power_law
 * Use exponent 0,5
 */
void light_set(enum e_led led, uint8_t level, enum light_mode mode)
{
	uint8_t new_level = ((uint16_t)level*level)/256;

	if (led >= LEDS_COUNT)
		return;

	leds_setup[led].level = (new_level == 0 && level != 0) ? 1 : new_level;
	leds_setup[led].mode = (mode == MODE_BLINK) ? MODE_NORMAL : mode;
}


/*
 * Set light output to given level if possible, for e.g. small leds, only
 * off and on states are possible (false/true)
 *
 * No linearization is applied
 */
void light_set_simple(enum e_led led, uint8_t level, enum light_mode mode)
{
	if (led >= LEDS_COUNT)
		return;

	leds_setup[led].level = level;
	leds_setup[led].mode = (mode == MODE_BLINK) ? MODE_NORMAL : mode;
}

/*
 * Get LED status
 */
enum light_mode light_get(enum e_led led)
{
	if (led >= LEDS_COUNT)
		return -1;

	if (leds_setup[led].level == 0)
		return MODE_OFF;
	return leds_setup[led].mode;
}

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
void light_blink(enum e_led led, uint8_t on_time, uint8_t period, uint8_t times)
{
	int i;

	if (led >= LEDS_COUNT || times == 0)
		return;

	/* if there's a blink running, unset it */
	for (i = 0; i < LEDS_COUNT; i++) {
		if (leds_setup[i].mode == MODE_BLINK) {
			leds_setup[i].mode = blink.prev_mode;
			break;
		}
	}

	if (on_time >= period)
		on_time = period/2;

	blink.prev_mode = leds_setup[led].mode;
	leds_setup[led].mode = MODE_BLINK;

	blink.period = period;
	blink.on_time = on_time;
	blink.count = times;
	blink.timer = 0;

	if (leds_setup[led].level == 0) {
		leds_setup[led].level = 128;
		blink.prev_mode = MODE_OFF;
	}
}

/*
 * True if there's no blink mode running
 */
uint8_t light_get_blink_finished(void)
{
	if (blink.count == 0)
		return 1;
	return 0;
}
