/*
 * System control functions
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2017
 */

#include <inttypes.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "hal/sleep.h"
#include "hal/leds.h"
#include "hal/int.h"

#include "buttons.h"

/*
 * Temporarily turn off all leds and send device to sleep mode
 *
 * When state of button is changed (pressed or released), it wakes from sleep
 * check the button and if it's pressed, returns from the function. If not,
 * will go to sleep again
 *
 * Leds will be reenabled in next main loop cycle automatically
 *
 * Shortest detectable button press is 11 ms
 */
void power_off(enum button btn)
{
	uint8_t pcmsk_num = btn == BUTTON1 ? BUTTON1_INT : BUTTON2_INT;
	//will be reenabled during next light_update call if needed
	leds_disable();
	led_sm1_off();
	led_sm2_off();

	while (1) {
		wdt_reset();
		int_pcmsk_enable(pcmsk_num);
		mcu_power_off();
		int_pcmsk_disable(pcmsk_num);

		_delay_ms(1);//time for bit of bouncing
		buttons_read();
		_delay_ms(10);
		buttons_read();

		//if still pressed, wake up, if not, go to sleep again
		if (button_state(btn) == BUTTON_PRESSED)
			break;
	}
}

