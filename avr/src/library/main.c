/*
 * Darkstar Mk.II
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 *
// first board revision - need to use reset pin, with reset disabled, won't be able to ISP
 */

#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#include "config.h"
#include "hal/clock.h"
#include "hal/uart.h"
#include "hal/leds.h"
#include "hal/timer.h"
#include "hal/adc.h"

#include "buttons.h"
#include "light.h"

#define WDT_EEPROM_ADDR (E2END-1)

extern void init(void);
extern void loop(void);

static uint16_t latest_run;

//updated every loop
static uint16_t sys_voltage;
static int8_t sys_temp;

ISR(BADISR_vect)
{
	//if bad interrupt was called, loop until wdt triggers reset
	while (1)
		;
}

/*
 * Get amount of wdt errors
 */
static uint8_t wdt_err_count(void)
{
	uint8_t tmp = eeprom_read_byte((void *)WDT_EEPROM_ADDR);
	//workaround for noninitialized memory
	if (tmp == 0xff)
		return 0;
	return tmp;
}

/*
 * Log wdt error
 */
static void wdt_err_inc(void)
{
	uint8_t latest = wdt_err_count();
	if (latest == 0xfe)
		return;

	eeprom_write_byte((void *)WDT_EEPROM_ADDR, latest+1);
}

/*
 * Run system tasks and user mode loop()
 */
static void main_loop(void)
{
	sys_voltage = adc_read_vcc();
	sys_temp = adc_core_temp();
	buttons_read();

	//call user script
	loop();

	light_update();
	wdt_reset();
}

/*
 * Get MCU core temperature in degrees C
 */
int8_t system_temp(void)
{
	return sys_temp;
}

/*
 * Get power supply voltage
 */
uint16_t system_voltage(void)
{
#ifdef VOLTAGE_DIFF_ENABLE
	return sys_voltage + VOLTAGE_DIFF;
#else
	return sys_voltage;
#endif
}

/*
 * Run system tasks, equivalent to returning from mode loop() with exception
 * after the tasks are finished program continues from place system_loop() was
 * called from
 */
void system_loop(void)
{
	while (time_diff(latest_run, millis()) < SYS_TICK)
		;
	sys_voltage = adc_read_vcc();
	sys_temp = adc_core_temp();

	buttons_read();
	light_update();
	wdt_reset();
	latest_run = millis();
}


int main(void)
{
	//if last reset was caused by wdt, log it
	if (MCUSR & WDRF) {
		MCUSR &= ~ WDRF;
		wdt_disable();
		wdt_err_inc();
	}

	//workaround for fast clock on voltages over 3
	//different value might be needed for different pieces of MCU
	OSCCAL0 = 0;

	//switch to 4MHz
	sys_clk_presc(CLK_PRES_2);


#ifdef UART_ENABLED
	uart_init();

	sei();
	fputs("DarkStar Mk.II\n", stdout);
	fputs("FW version: " FW_VERSION"\n", stdout);
	fputs("HW revision: " HW_VERSION"\n", stdout);
	fputs("Built on: "__DATE__"\n\n", stdout);
#endif

	sei();

	wdt_reset();
	wdt_enable(WDT_TIMEOUT);

	time_init();
	adc_init();
	leds_init();
	adc_pin_set(PHOTOTRANS_ADC);

	//read current state of buttons 2 times to get the pressed state
	//correctly. Useful for handling button pressed while battery is being connected
	buttons_read();
	buttons_read();
	buttons_read();

	//initialize user mode
	init();

	while (1) {
		main_loop();
		latest_run = millis();
		while (time_diff(latest_run, millis()) < SYS_TICK)
			;
	}

	return 0;
}
