/*
 * Interrupts
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2017
 */

#ifndef __INT_H_
#define __INT_H_

#include <avr/io.h>
#include <inttypes.h>

enum int0_type {
	//interrupt on low level, will trigger until level is high
	INT0_LOW_LEVEL = 0,
	//interrupt on any edge
	INT0_ANY_CHANGE,
	//failing edge interrupt
	INT0_FAILING,
	//rising edge interrupt
	INT0_RISING
};

extern void int0_enable(enum int0_type type);

#define int0_disable() { GIMSK &= ~_BV(INT0); }

/*
 * Enable PCINTx change interrupt
 */
extern void int_pcmsk_enable(uint8_t num);

/*
 * Disable interrupt on PCINTx change, not only mask is changed, but while
 * port interrupt is disabled
 */
extern void int_pcmsk_disable(uint8_t num);
#endif
