/*
 * Caving mode
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2017
 */

#include <inttypes.h>
#include <string.h>

#include "system.h"
#include "buttons.h"
#include "light.h"

struct s_levels {
	uint8_t flood;
	uint8_t spot;
	uint8_t red;
};

struct s_config {
	uint8_t num_levels;
	uint8_t prg_locked : 1;
	uint8_t light_control : 2;
};

enum e_mode {
	NORMAL,
	PROGRAMMING,
	LOCKED,
	CONFIGURATION,
};

#define BUTTON_UP BUTTON1
#define BUTTON_DOWN BUTTON2

//time in tenths of ms to be considered as holding
#define HOLD_TIME 100

//time spent in mode=0 to go to sleep
#define OFF_TIME 200

//voltage levels to report
#define VOLTAGE_5	3600
#define VOLTAGE_4	3500
#define VOLTAGE_3	3300	//limit 1
#define VOLTAGE_2	3200	//limit 2
#define VOLTAGE_1	3100	//limit 3

//temperature limits
#define TEMP_LIM_START	40	//enable limiting above this temperature
#define TEMP_LIM_AGGRESIVE	50	//limit more aggresive above this value
#define TEMP_LIM_MIN	30	//don't limit below this level

//minimum settable light level
#define LIGHT_MIN 5
//increase by this value in programming mode
#define LIGHT_STEP 50

#define DEF_LEVELS 7
#define MAX_LEVELS 10
const struct s_levels def_levels[] = {{0, 0, 0}, // off - must be here
				      {0, 0, 1}, //red only
				      {LIGHT_MIN, 0, 0}, //low spot
				      {LIGHT_STEP, LIGHT_STEP, 0}, //small spaces
				      {100, 50, 0}, //normal walking
				      {70, 200, 0}, //distance
				      {200, 200, 0}}; //photo

//limit levels for low voltage
const uint8_t limits[4] = {255, 200, 150, 100};

static struct s_levels levels[MAX_LEVELS];
static struct s_config config = { DEF_LEVELS, 0, MODE_NORMAL };

static uint8_t cur_levels;
static enum e_mode cur_mode;

static uint8_t hold_done;
static uint8_t timer;

static void config_load(void)
{
	config_read(0, (uint8_t *)&config, sizeof(config));
	config_read(sizeof(config), (uint8_t *)&levels, sizeof(levels[0])*config.num_levels);
}

static void config_save(void)
{
	config_write(0, (uint8_t *)&config, sizeof(config));
	config_write(sizeof(config), (uint8_t *)&levels, sizeof(levels[0])*config.num_levels);
}

static void config_default(void)
{
	memcpy(&levels, &def_levels, DEF_LEVELS*sizeof(def_levels[0]));
	config.num_levels = DEF_LEVELS;
	config_save();
}

/*
 * Blink remaining charge
 */
static void report_voltage(void)
{
	uint16_t voltage = system_voltage();

	if (voltage < VOLTAGE_1)
		light_blink(LED_FLOOD, 30, 60, 1);
	else if (voltage < VOLTAGE_2)
		light_blink(LED_FLOOD, 30, 60, 2);
	else if (voltage < VOLTAGE_3)
		light_blink(LED_FLOOD, 30, 60, 3);
	else if (voltage < VOLTAGE_4)
		light_blink(LED_FLOOD, 30, 60, 4);
	else if (voltage < VOLTAGE_5)
		light_blink(LED_FLOOD, 30, 60, 5);
	else
		light_blink(LED_FLOOD, 30, 60, 6);
}

/*
 * Check if supply voltage is below certain limits, if so, notify user if
 * haven't before
 */
static uint8_t check_voltage(void)
{
	static uint8_t reported = 0;
	uint16_t voltage = system_voltage();

	if (voltage > VOLTAGE_3)
		reported = 0;

	if (voltage < VOLTAGE_1 && reported < 3) {
		light_blink(LED_FLOOD, 30, 60, 3);
		reported = 3;
	} else if (voltage < VOLTAGE_2 && reported < 2) {
		light_blink(LED_FLOOD, 30, 60, 2);
		reported = 2;
	} else if (voltage < VOLTAGE_3 && reported < 1) {
		light_blink(LED_FLOOD, 30, 60, 1);
		reported = 1;
	}

	return reported;
}

/*
 * Reduce output power when overheating
 */
static uint8_t heat_limit(void)
{
	int8_t temp = system_temp();
	uint8_t tmp;

	if (temp < TEMP_LIM_START)
		return 255;

	if (temp < TEMP_LIM_AGGRESIVE)
		return 255 - (temp - TEMP_LIM_START + 1)*4;

	tmp = (temp-TEMP_LIM_AGGRESIVE)*8;
	if (tmp + TEMP_LIM_MIN > 128)
		return TEMP_LIM_MIN;
	return 128 - tmp;
}

static void mode_programming(void)
{
	if (cur_levels == 0) {
		cur_mode = NORMAL;
		return;
	}

	if (button_state(BUTTON_UP) == BUTTON_JUST_RELEASED &&
	    button_pressed_time(BUTTON_UP) <= HOLD_TIME) {
		if (levels[cur_levels].flood > 255-LIGHT_STEP)
			levels[cur_levels].flood = 0;
		else if (levels[cur_levels].flood == 0)
			levels[cur_levels].flood = LIGHT_MIN;
		else
			levels[cur_levels].flood += LIGHT_STEP;
	}

	if (button_state(BUTTON_DOWN) == BUTTON_JUST_RELEASED &&
	    button_pressed_time(BUTTON_DOWN) <= HOLD_TIME) {
		if (levels[cur_levels].spot > 255-LIGHT_STEP)
			levels[cur_levels].spot = 0;
		else if (levels[cur_levels].spot == 0)
			levels[cur_levels].spot = LIGHT_MIN;
		else
			levels[cur_levels].spot += LIGHT_STEP;
	}

	if (button_state(BUTTON_DOWN) == BUTTON_PRESSED &&
	    button_pressed_time(BUTTON_DOWN) >= HOLD_TIME && !hold_done) {
		hold_done = 1;
		levels[cur_levels].red = levels[cur_levels].red == 1 ? 0 : 1;
	}

	/*
	if (button_state(BUTTON_UP) == BUTTON_PRESSED &&
	    button_pressed_time(BUTTON_UP) >= HOLD_TIME && !hold_done) {
		hold_done = 1;
		levels[cur_levels].white = levels[cur_levels].white == 1 ? 0 : 1;
	}
	*/

	if (button_state(BUTTON_UP) == BUTTON_RELEASED &&
	    button_state(BUTTON_DOWN) == BUTTON_RELEASED) {
		hold_done = 0;
		timer++;
		if (timer == 0xff) {
			cur_mode = NORMAL;
			light_blink(LED_SPOT, 50, 100, 1);
			config_save();
			return;
		}
	} else {
		timer = 0;
	}

	light_set(LED_FLOOD, levels[cur_levels].flood, MODE_NORMAL);
	light_set(LED_SPOT, levels[cur_levels].spot, MODE_NORMAL);
	light_set(LED_RED, levels[cur_levels].red, MODE_NORMAL);
}

/*
 * Setup number of modes, programming mode, lock enable
 */
static void mode_config(void)
{
	uint8_t changed = 0;
	uint8_t i = 0;

	if (button_state(BUTTON_UP) == BUTTON_JUST_RELEASED &&
	    button_pressed_time(BUTTON_UP) <= HOLD_TIME) {
		config.num_levels = config.num_levels == MAX_LEVELS ? MAX_LEVELS : config.num_levels+1;
		changed = 1;
	}

	if (button_state(BUTTON_DOWN) == BUTTON_JUST_RELEASED &&
	    button_pressed_time(BUTTON_DOWN) <= HOLD_TIME) {
		config.num_levels = config.num_levels == 1 ? 1 : config.num_levels-1;
		changed = 1;
	}

	if (button_state(BUTTON_UP) == BUTTON_PRESSED &&
	    button_pressed_time(BUTTON_UP) >= HOLD_TIME && !hold_done) {
		hold_done = 1;
		changed = 1;
		//programming mode enable/disable
		if (config.prg_locked) {
			config.prg_locked = 0;
			light_blink(LED_RED, 25, 50, 2);
		} else {
			config.prg_locked = 1;
			light_blink(LED_RED, 25, 50, 1);

		}
	}

	if (button_state(BUTTON_DOWN) == BUTTON_PRESSED &&
	    button_pressed_time(BUTTON_DOWN) >= HOLD_TIME && !hold_done) {
		hold_done = 1;
		light_blink(LED_RED, 25, 50, config.num_levels);
	}

	if (button_state(BUTTON_UP) == BUTTON_RELEASED &&
	    button_state(BUTTON_DOWN) == BUTTON_RELEASED) {
		hold_done = 0;
		timer++;
	} else {
		timer = 0;
	}

	if (light_get_blink_finished())
		light_set(LED_RED, 1, MODE_NORMAL);

	if (changed == 1 && timer == 0xff) {
		for (i = 1; i < config.num_levels; i++) {
			if (levels[i].red == 0 && levels[i].spot == 0 && levels[i].flood == 0) {
				levels[i].spot = 50;
				levels[i].red = i % 2;
			}
		}
		changed = 0;
		config_save();
	}
}

/*
 * Transport lock, sweep knob back and forth to disable
 */
static void mode_locked(void)
{
	static uint8_t latest = 0;

	if (button_state(BUTTON_UP) == BUTTON_JUST_RELEASED &&
	  button_pressed_time(BUTTON_UP) < HOLD_TIME) {
		latest = (latest << 2) | 0x03;
		timer = 0;
	} else if (button_state(BUTTON_DOWN) == BUTTON_JUST_RELEASED &&
	   button_pressed_time(BUTTON_DOWN) < HOLD_TIME) {
		latest <<= 2;
		timer = 0;
	}

	//waiting for 11001100
	if (latest == 0xCC) {
		cur_mode = NORMAL;
		cur_levels = 1;
		timer = 0;
		latest = 0;
	}
	//reset if buttons not touched for 2,55 seconds
	if (++timer == 0xff) {
		latest = 0;
		power_off(BUTTON_UP|BUTTON_DOWN);
	}
}

/*
 * Usual lamp operations
 */
static void mode_normal(void)
{
	if (button_state(BUTTON_UP) == BUTTON_JUST_RELEASED &&
	    button_pressed_time(BUTTON_UP) <= HOLD_TIME)
		cur_levels = (cur_levels + 1)%config.num_levels;

	if (button_state(BUTTON_DOWN) == BUTTON_JUST_RELEASED &&
	    button_pressed_time(BUTTON_DOWN) <= HOLD_TIME)
		cur_levels = cur_levels == 0 ? config.num_levels : cur_levels-1;

	if (button_state(BUTTON_DOWN) == BUTTON_PRESSED &&
	    button_pressed_time(BUTTON_DOWN) >= HOLD_TIME && !hold_done) {
		hold_done = 1;
		//lamp off and holding - enable transport lock
		if (cur_levels == 0) {
			cur_mode = LOCKED;
			light_blink(LED_SPOT, 30, 60, 3);
		//switch led control modes
		} else if (config.light_control == MODE_NORMAL) {
			light_blink(LED_SPOT, 50, 100, 2);
			config.light_control = MODE_AUTO;
			config_save();
		} else {
			light_blink(LED_SPOT, 50, 100, 1);
			config.light_control = MODE_NORMAL;
			config_save();
		}
	}

	if (button_state(BUTTON_UP) == BUTTON_PRESSED &&
	    button_pressed_time(BUTTON_UP) >= HOLD_TIME && !hold_done) {
		hold_done = 1;
		if (cur_levels == 0) {
			report_voltage();
		} else if (config.prg_locked == 0) {
			cur_mode = PROGRAMMING;
			light_blink(LED_SPOT, 50, 100, 1);
		}
	}

	if (button_state(BUTTON_UP) == BUTTON_RELEASED &&
	    button_state(BUTTON_DOWN) == BUTTON_RELEASED)
		hold_done = 0;

	//apply changes from above
	if (light_get_blink_finished()) {
		if (cur_levels != 0) {
			uint8_t min = limits[check_voltage()];
			uint8_t tmp = heat_limit();
			min = tmp < min ? tmp : min;
			light_set_limit(min);
		}

		light_set(LED_FLOOD, levels[cur_levels].flood, config.light_control);
		light_set(LED_SPOT, levels[cur_levels].spot, config.light_control);
		light_set(LED_RED, levels[cur_levels].red, MODE_NORMAL);
	}

	//turn off lamp if all leds are of for more than OFF_TIME
	if (cur_levels == 0 && button_state(BUTTON_DOWN) == BUTTON_RELEASED &&
	    button_state(BUTTON_UP) == BUTTON_RELEASED &&
	    light_get_blink_finished()) {
		timer++;
		if (timer > OFF_TIME) {
			power_off(BUTTON_UP|BUTTON_DOWN);
			timer = 0;
		}
	} else {
		timer = 0;
	}
}

/*
 * This function will be called after connecting power to headlamp
 */
void init(void)
{
	uint8_t c;

	/*
	config_read(0, &c, 1);
	if (c == 0 || c > 10)
		config_default();
	else
		config_load();
		*/

	cur_levels = 0;
	cur_mode = NORMAL;
	hold_done = 0;
	timer = 0;

	if (button_state(BUTTON_DOWN) == BUTTON_PRESSED)
		cur_mode = CONFIGURATION;

	config_default();
}


/*
 * This function is called periodically every 10ms
 *
 * It can't be blocking, just sweep throught as fast as possible
 */
void loop(void)
{
	switch (cur_mode) {
	case PROGRAMMING:
		mode_programming();
		break;
	case LOCKED:
		mode_locked();
		break;
	case CONFIGURATION:
		mode_config();
		break;
	default:
		mode_normal();
		break;
	}
}
