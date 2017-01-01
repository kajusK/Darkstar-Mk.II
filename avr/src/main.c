/*
 * Darkstar Mk.II
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 *
 * TODO FUSE - ckdiv8 set, shipped with set -> 1MHz
// The BOD mode of operation is selected using BODACT and BODPD fuse bits. - disable BOD in sleep
// TODO handle various resets, log the errors
// handle processing eeprom - configuration, most used modes,...
 */

#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "config.h"
#include "hal/clock.h"
#include "hal/uart.h"
#include "hal/leds.h"
#include "hal/timer.h"
#include "hal/adc.h"

#include "buttons.h"
#include "light.h"

ISR(BADISR_vect)
{

}

void main_loop(void)
{
	buttons_read();

	//call_user_script

	light_update();
	wdt_reset();
}


int main(void)
{
	uint16_t latest_run;

	wdt_reset();
	wdt_enable(WDT_TIMEOUT);

	//switch to 4MHz
	sys_clk_presc(CLK_PRES_2);

	// after watchdog reset clear reset flag and increment wdt reset counter
	//the application software should always clear the WDRF flag and the WDE control bit in the initialization routine.
	//MCUSR - contains reset flags

	uart_init();

	sei();
	fputs("DarkStar Mk.II\n", stdout);
	fputs("FW version: " FW_VERSION"\n", stdout);
	fputs("HW revision: " HW_VERSION"\n", stdout);
	fputs("Built on: "__DATE__"\n\n", stdout);

	time_init();
	adc_init();
	leds_init();
	adc_pin_set(PHOTOTRANS_ADC);

	while(1) {
		main_loop();
		latest_run = millis();
		while (time_diff(latest_run, millis()) < SYS_TICK)
			;
	}

	return 0;
}
