/*
 * System clock
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#include <avr/io.h>
#include <util/atomic.h>
#include <inttypes.h>

#include "hal/clock.h"

/*
 * Set system clock prescaler
 *
 * The device is shipped with the CKDIV8 Fuse programmed - CLK_PRES_8 is set
 */
void sys_clk_presc(enum sys_clk_presc prescaler)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		//magical constant to unlock CLKPR for 4 cycles
		CCP = 0xD8;
		CLKPR = prescaler;
	}
}

/*
 * Set system clock source, wait until stabilized
 */
void sys_clk_source(enum sys_clk_source source)
{
	loop_until_bit_is_set(CLKCR, OSCRDY);

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		CCP = 0xD8;
		CLKCR = source;
	}
}
