/*
 * Serial communications using stdin/out, stderr is redirected to stdout
 * Global interrupt must be enabled
 *
 * Using one stop bit and no parity
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#ifndef _UART_H_
#define _UART_H_

//BAUD and F_CPU must be defined
//#define BAUD	9600

// uncomment to use serial port 1 instead of 0 if present
// #define UART_USE_1

// uncomment to use binary safe version (won't send \r after \n)
// #define UART_BINARY_SAFE

/*
 * check if there are data in input queue
 *
 * returns 0 if empty, 1 otherwise
 */
extern int uart_check_rx(void);

/*
 * Open stdin/err/out descriptors, setup avr hardware...
 */
extern void uart_init(void);

#endif
