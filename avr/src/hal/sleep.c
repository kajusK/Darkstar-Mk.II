/*
 * Sleep modes
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "hal/adc.h"
#include "config.h"

/*
 * Power off the device, only INT0, UART, TWI or Interrupt on pin change
 * can wake it up again
 */
void power_off(void)
{
	adc_disable();

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	wdt_disable();

	sleep_enable();
#ifdef sleep_bod_disable
	sleep_bod_disable();
#endif
	sei();		//just to be sure we'll be able to up
	sleep_cpu();
	sleep_disable();

	wdt_enable(WDT_TIMEOUT);
	adc_enable();
}
