/*
 * Simple flashlight control mode
 *
 * Holding UP button turns headlamp on/off, pressing UP button browses
 * FLOOD led levels, DOWN controls SPOT levels
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2017
 */

#include "system.h"
#include "buttons.h"
#include "light.h"

uint8_t led1_mode;
uint8_t led2_mode;
uint8_t timer;

#define BUTTON_UP BUTTON1
#define BUTTON_DOWN BUTTON2

//time in tenths of ms to be considered as holding
#define HOLD_TIME 100
//if both leds are of for more than 2s, turn off
#define OFF_TIME 200

#define LIGHT_STEPS 5
uint8_t light_output[LIGHT_STEPS] = {0, 1, 50, 150, 200};

/*
 * This function will be called after connecting power to headlamp
 *
 * By default, everything should be off
 */
void init(void)
{
	led1_mode = 0;
	led2_mode = 0;
	timer = 0;
}

/*
 * This function is called periodically every 10ms
 *
 * It can't be blocking, just sweep throught as fast as possible
 */
void loop(void)
{
	//will be turning off
	if (button_state(BUTTON_UP) == BUTTON_PRESSED &&
	    button_pressed_time(BUTTON_UP) >= HOLD_TIME) {
		light_off();
		return;
	}

	if (button_state(BUTTON_UP) == BUTTON_JUST_RELEASED) {
		//turn off, holding to long
		if (button_pressed_time(BUTTON_UP) > HOLD_TIME) {
			power_off(BUTTON_UP);
			if (led1_mode == 0 && led2_mode == 0) {
				led1_mode = 1;
			}
			light_set(LED_FLOOD, led1_mode, MODE_NORMAL);
			light_set(LED_SPOT, led2_mode, MODE_NORMAL);
			return;
		}

		led1_mode = (led1_mode + 1) % LIGHT_STEPS;
	}

	if (button_state(BUTTON_DOWN) == BUTTON_JUST_RELEASED) {
		led2_mode = (led2_mode + 1) % LIGHT_STEPS;
	}

	light_set(LED_FLOOD, light_output[led1_mode], MODE_NORMAL);
	light_set(LED_SPOT, light_output[led2_mode], MODE_NORMAL);

	//turn off lamp if both leds are of for more than OFF_TIME
	if (led1_mode == 0 && led2_mode == 0) {
		timer++;
		if (timer > OFF_TIME) {
			power_off(BUTTON_UP);
			led1_mode = 1;
			light_set(LED_FLOOD, led1_mode, MODE_NORMAL);
			timer = 0;
		}
	} else {
		timer = 0;
	}
}
