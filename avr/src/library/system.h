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

#include "buttons.h"
#include "config.h"

/*
 * Get MCU core temperature in degrees C
 */
extern int8_t system_temp(void);

/*
 * Get power supply voltage
 */
extern uint16_t system_voltage(void);

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

#ifdef PHOTOTRANS_ADC
/*
 * Get amount of light in surrounding area
 */
extern uint8_t system_light(void);
#endif

/*
 * Write data to eeprom
 */
extern void config_write(uint16_t addr, uint8_t *buf, uint8_t count);

/*
 * Read data from eeprom
 */
extern void config_read(uint16_t addr, uint8_t *buf, uint8_t count);

/*
 * Return time in milliseconds since the init_time was called
 *
 * Overflows every 2^16 millisecond ~ 65 seconds
 */
uint16_t millis(void);

#endif
