/*
 * Simple flashlight control mode
 *
 * Holding UP button turns headlamp on/off, pressing UP button browses
 * FLOOD led levels, DOWN controls SPOT levels. Long DOWN press toggles red LED
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2017
 */

#include "system.h"
#include "buttons.h"
#include "light.h"

struct s_mode {
	uint8_t spot;
	uint8_t flood;
	uint8_t red;
};

static struct s_mode leds;
static struct s_mode leds_save;

static uint8_t timer;
static uint8_t hold_done;

static uint8_t in_sleep;

#define BUTTON_UP BUTTON1
#define BUTTON_DOWN BUTTON2

//time in tenths of ms to be considered as holding
#define HOLD_TIME 100
//if all leds are of for more than 2s, turn off
#define OFF_TIME 200

#define LIGHT_STEPS 6
uint8_t light_output[LIGHT_STEPS] = {0, 5, 40, 100, 150, 200};

/*
 * This function will be called after connecting power to headlamp
 *
 * By default, everything should be off
 */
void init(void)
{
	leds.spot = 0;
	leds.flood = 0;
	leds.red = 0;
	leds_save.spot = 0;
	leds_save.flood = 0;
	leds_save.red = 0;

	hold_done = 0;
	in_sleep = 1;
	timer = 0;
}

/*
 * This function is called periodically every 10ms
 *
 * It can't be blocking, just sweep throught as fast as possible
 */
void loop(void)
{
	//Control RED led
	if (button_state(BUTTON_DOWN) == BUTTON_PRESSED &&
	    button_pressed_time(BUTTON_DOWN) >= HOLD_TIME && !hold_done) {
			hold_done = 1;
			leds.red = (leds.red + 1) % 2;
			if (leds.red != 0)
				in_sleep = 0;
	}

	//will be turning off
	if (button_state(BUTTON_UP) == BUTTON_PRESSED &&
	    button_pressed_time(BUTTON_UP) >= HOLD_TIME && !hold_done) {
		hold_done = 1;

		if (in_sleep) {
			in_sleep = 0;
			leds = leds_save;
			if (leds.red == 0 && leds.spot == 0 && leds.flood == 0)
				leds.flood = 1;
		} else {
			in_sleep = 1;

			//save current settings
			leds_save = leds;
			leds.flood = 0;
			leds.spot = 0;
			leds.red = 0;
		}
	}

	if (!in_sleep) {
		if (button_state(BUTTON_UP) == BUTTON_JUST_RELEASED &&
		    button_pressed_time(BUTTON_UP) <= HOLD_TIME)
			leds.spot = (leds.spot + 1) % LIGHT_STEPS;

		if (button_state(BUTTON_DOWN) == BUTTON_JUST_RELEASED &&
		    button_pressed_time(BUTTON_DOWN) <= HOLD_TIME)
			leds.flood = (leds.flood + 1) % LIGHT_STEPS;
	}


	light_set_simple(LED_FLOOD, light_output[leds.flood], MODE_NORMAL);
	light_set_simple(LED_SPOT, light_output[leds.spot], MODE_NORMAL);
	light_set_simple(LED_RED, leds.red, MODE_NORMAL);

	if (button_state(BUTTON_DOWN) == BUTTON_RELEASED &&
	    button_state(BUTTON_UP) == BUTTON_RELEASED) {
		hold_done = 0;

		//turn off lamp if both leds are of for more than OFF_TIME
		if (leds.flood == 0 && leds.spot == 0 && leds.red == 0) {
			timer++;
			if (timer > OFF_TIME) {
				if (in_sleep == 0) {
					in_sleep = 1;
					leds_save = leds;
				}
				power_off(BUTTON_UP|BUTTON_DOWN);
				timer = 0;
				return;
			}
		}
	} else {
		timer = 0;
	}
}
