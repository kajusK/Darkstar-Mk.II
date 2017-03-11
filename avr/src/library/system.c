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
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "hal/sleep.h"
#include "hal/leds.h"
#include "hal/int.h"

#include "buttons.h"

//empty interrupts, used to wake up only
ISR(PCINT0_vect)
{
}

ISR(PCINT1_vect)
{
}

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
 *
 * Button can be BUTTON1, BUTTON2 or BUTTON1|BUTTON2 for both
 */
void power_off(uint8_t btn)
{
	//if control mode was written by asshole, give him a way to wake up
	//when no button was set
	if (!(btn & (BUTTON1 | BUTTON2)))
		btn = BUTTON1;

	//will be reenabled during next light_update call if needed
	leds_disable();
	led_sm1_off();
	led_sm2_off();

	while (1) {
		wdt_reset();

		if (btn & BUTTON1)
			int_pcmsk_enable(BUTTON1_INT);
		if (btn & BUTTON2)
			int_pcmsk_enable(BUTTON2_INT);

		//go to sleep
		mcu_power_off();

		if (btn & BUTTON1)
			int_pcmsk_disable(BUTTON1_INT);
		if (btn & BUTTON2)
			int_pcmsk_disable(BUTTON2_INT);

		//time for bit of bouncing
		_delay_ms(10);
		buttons_read();
		_delay_ms(10);
		buttons_read();
		buttons_read();

		//if still pressed, wake up, if not, go to sleep again
		if (btn & BUTTON1 && button_state(BUTTON1) == BUTTON_PRESSED)
			break;
		if (btn & BUTTON2 && button_state(BUTTON2) == BUTTON_PRESSED)
			break;
	}
}

/*
 * Read data from eeprom
 */
void config_read(uint16_t addr, uint8_t *buf, uint8_t count)
{
	eeprom_read_block(buf, (void *)addr, count);
}

/*
 * Write data to eeprom
 */
void config_write(uint16_t addr, uint8_t *buf, uint8_t count)
{
	eeprom_update_block(buf, (void *)addr, count);
}
