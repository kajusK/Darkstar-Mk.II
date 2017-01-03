/*
 * Interrupts
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2017
 */

#include <avr/io.h>
#include <inttypes.h>

#include "hal/int.h"

void int0_enable(enum int0_type type)
{
	MCUCR &= ~(0x03);
	MCUCR |= type;
	GIFR &= ~_BV(INTF0);
	GIMSK |= _BV(INT0);
}

/*
 * Enable PCINTx change interrupt
 */
void int_pcmsk_enable(uint8_t num)
{
	//set mask
	if (num >= 8) {
		PCMSK1 |= _BV(num - 8);
		GIFR &= ~_BV(PCIF1);
		GIMSK |= _BV(PCIE1);
		return;
	}

	PCMSK0 |= _BV(num);
	GIFR &= ~_BV(PCIF0);
	GIMSK |= _BV(PCIE0);
}

/*
 * Disable interrupt on PCINTx change, not only mask is changed, but while
 * port interrupt is disabled
 */
void int_pcmsk_disable(uint8_t num)
{
	if (num >= 8) {
		PCMSK1 &= ~_BV(num - 8);
		GIMSK &= ~_BV(PCIE1);
		return;
	}

	PCMSK0 &= ~_BV(num);
	GIMSK &= ~_BV(PCIE0);
}
