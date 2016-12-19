/*
 * Darkstar Mk.II
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 *
 * TODO FUSE - ckdiv8 set, shipped with set -> 1MHz
 * PA7,PA5 high sink  PHDE register enable
 */

#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>

#include "config.h"
#include "hal/uart.h"

ISR(BADISR_vect)
{

}

int main(void)
{
	// The BOD mode of operation is selected using BODACT and BODPD fuse bits. - disable BOD in sleep
	// after watchdog reset clear reset flag and increment wdt reset counter
	//the application software should always clear the WDRF flag and the WDE control bit in the initialization routine.
	//MCUSR - contains reset flags

	uart_init();

	sei();
	fputs("DarkStar Mk.II\n", stdout);
	fputs("FW version: " FW_VERSION"\n", stdout);
	fputs("HW revision: " HW_VERSION"\n", stdout);
	fputs("Built on: "__DATE__"\n\n", stdout);

	return 0;
}
