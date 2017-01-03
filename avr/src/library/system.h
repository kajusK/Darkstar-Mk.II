/*
 * System control functions
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2017
 */

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#include "hal/adc.h"
#include "config.h"
#include "buttons.h"

#define system_temp() adc_core_temp();

#ifdef SUPPLY_DIODE
	#define system_voltage() (adc_read_vcc() - DIODE_FORWARD_VOLTAGE);
#else
	#define system_voltage() adc_read_vcc();
#endif

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
extern void power_off(enum button btn);

#endif
