/*
 * Testing lamp functionality
 *
 * when up/down is pressed, red/white led is on
 * short press up/down LED1/2 is changed
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2017
 */

#include <stdio.h>
#include <avr/wdt.h>

#include "system.h"
#include "buttons.h"
#include "light.h"

#include "utils/ram.h"

#include "config.h"
#include <util/delay.h>
uint8_t led1_mode;
uint8_t led2_mode;

#define BUTTON_UP BUTTON1
#define BUTTON_DOWN BUTTON2

//time in tenths of ms to be considered as holding
#define HOLD_TIME 100
//if both leds are of for more than 2s, turn off

#define LIGHT_STEPS 6

uint8_t light_output[LIGHT_STEPS] = {0, 15, 50, 150, 240, 255};

/*
 * This function will be called after connecting power to headlamp
 *
 * By default, everything should be off
 */
void init(void)
{
#ifdef UART_ENABLED
	//serial is slow, it would trigger wdt reset
	wdt_disable();
#endif

	led1_mode = 0;
	led2_mode = 0;
}

/*
 * This function is called periodically every 10ms
 *
 * It can't be blocking, just sweep throught as fast as possible
 */
void loop(void)
{
	if (button_state(BUTTON_UP) == BUTTON_PRESSED)
		light_set(LED_RED, 1, MODE_NORMAL);
	else
		light_set(LED_RED, 0, MODE_NORMAL);

	if (button_state(BUTTON_DOWN) == BUTTON_PRESSED)
		light_set(LED_WHITE, 1, MODE_NORMAL);
	else
		light_set(LED_WHITE, 0, MODE_NORMAL);

	if (button_state(BUTTON_UP) == BUTTON_JUST_RELEASED) {
		if (button_pressed_time(BUTTON_UP) > HOLD_TIME) {
			light_blink(LED_SPOT, 100, 200, 3);
		} else {
			led1_mode = (led1_mode + 1) % LIGHT_STEPS;
			light_set_simple(LED_SPOT, light_output[led1_mode], MODE_NORMAL);
		}
	}

	if (button_state(BUTTON_DOWN) == BUTTON_JUST_RELEASED) {
		if (button_pressed_time(BUTTON_DOWN) > HOLD_TIME) {
			light_blink(LED_FLOOD, 100, 200, 3);
		} else {
			led2_mode = (led2_mode + 1) % LIGHT_STEPS;
			light_set_simple(LED_FLOOD, light_output[led2_mode], MODE_CONSTANT);
		}
	}

#ifdef UART_ENABLED
	printf("\e[1;1H\e[2JBat: %d\n", system_voltage());
	printf("Temp: %d\n", adc_core_temp());
	printf("Light: %d\n", adc_read(PHOTOTRANS_ADC, AD_REF_1_1V));
	printf("Ram: %d\n", ram_free());
#endif
}
