/*
 * System control functions
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2017
 */

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#include <inttypes.h>

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
 * Run system tasks, equivalent to returning from mode loop() with exception
 * after the tasks are finished program continues from place system_loop() was
 * called from
 */
extern void system_loop(void);

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
extern void power_off(uint8_t btn);

#endif
