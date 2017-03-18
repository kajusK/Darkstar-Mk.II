/*
 * Main API configuration
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2017
 */

#ifndef __CONFIG_H_
#define __CONFIG_H_

#include <avr/wdt.h>

/* definitions to keep the things sane, don't modify */
#define PORT_(port)	PORT ## port
#define DDR_(port)	DDR  ## port
#define PIN_(port)	PIN  ## port

#define PORT(port)	PORT_(port)
#define DDR(port)	DDR_(port)
#define PIN(port)	PIN_(port)

#define FW_VERSION "0.1"
#define HW_VERSION "0.1"

/*
 *  Default values
 */
// enable serial
#define UART_ENABLED

// serial baudrate
#define BAUD	9600

#define WDT_TIMEOUT WDTO_30MS

//system loop is run every tick ms
#define SYS_TICK 10

//led current to set when using additional pwm dimming, 0-255, should be
//within reasonable led driver efficiency region
#define DEFAULT_DIM_CURRENT	32

//uncomment to calculate voltage decreased by forward voltage on diode
#define SUPPLY_DIODE
//forward voltage of diode on supply rail
#define DIODE_FORWARD_VOLTAGE 320

/*
 * Hard limits
 */
//maximum pwm value settable, e.g. when output must be limited to match
//maximum led current (0-255)
#define MAX_PWM		255

//output will be limited to minimal level after exceeding this temperature
#define TEMP_MAX	60
//main light will be shut down after exceeding this temperature
#define TEMP_SHUTDOWN	70
//hystersis for applying temp limits, only considered for
#define TEMP_HYSTERSIS	10
//maximum PWM duty when over TEMP_MAX
#define PWM_OVERHEAT_MAX 15

//voltage in mV to limit max output
#define VOLTAGE_MIN	3000
//main light will be shut down after reaching this limit
#define VOLTAGE_SHUTDOWN 2600
//voltage hystersis
#define VOLTAGE_HYSTERSIS 100
//maximum PWM duty when under VOLTAGE_MIN
#define PWM_UNDERV	15

/*
 * IO definitions
 */
// uncomment to use serial port 1, 0 used by default
// #define UART_USE_1

/*
 * TOCCx outputs for pwm + corresponding port/pin
 *
 * Due to internal limitations, one LEDx_PWM_NUM must be even, second must be
 * odd
 *
 * LED1 - SPOT
 * LED2 - FLOOD
 * LED_SM1 - RED
 * LED_SM2 - WHITE
 */
#define LED1_PWM_NUM	2
#define LED2_PWM_NUM	3

#define LED1_PWM_PORT	A
#define LED1_PWM_PIN	3
#define LED2_PWM_PORT	A
#define LED2_PWM_PIN	4

#define LED1_DIM_PORT	A
#define LED1_DIM_PIN	6
#define LED2_DIM_PORT	A
#define LED2_DIM_PIN	5

/* LED driver power transistor */
#define DRIVER_ENABLE_PORT	B
#define DRIVER_ENABLE_PIN	2

/* Small low-power leds */
#define LED_SM1_PORT	A
#define LED_SM1_PIN	7
//comment to disable - e.g if not present
//#define LED_SM2_PORT	A
//#define LED_SM2_PIN	0

/* Light measurement */
#define PHOTOTRANS_ADC	0

/* Buttons */
#define BUTTON1_PORT	B
#define BUTTON1_PIN	0
//PCINTx for corresponding pin
#define BUTTON1_INT	8

#define BUTTON2_PORT	B
#define BUTTON2_PIN	1
#define BUTTON2_INT	9

#endif
