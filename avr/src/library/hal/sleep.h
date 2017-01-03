/*
 * Sleep modes
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#ifndef __SLEEP_H_
#define __SLEEP_H_

/*
 * Power off the device, only INT0, UART, TWI or Interrupt on pin change
 * can wake it up again
 */
extern void mcu_power_off(void);

#endif
