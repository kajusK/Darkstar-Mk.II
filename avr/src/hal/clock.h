/*
 * System clock
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#ifndef __CLOCK_H_
#define __CLOCK_H_

enum sys_clk_presc {
	CLK_PRES_1 = 0,
	CLK_PRES_2,
	CLK_PRES_4,
	CLK_PRES_8,
	CLK_PRES_16,
	CLK_PRES_32,
	CLK_PRES_64,
	CLK_PRES_128,
	CLK_PRES_256,
};

enum sys_clk_source {
	CLK_SRC_EXT = 0,
	CLK_SRC_8MHZ = 2,
	CLK_SRC_32KHZ = 4,
	CLK_SRC_LFCO = 6,
	CLK_SRC_CR_1MHZ = 8,
	CLK_SRC_CR_3MHZ = 10,
	CLK_SRC_CR_8MHZ = 12,
	CLK_SRC_CR_16MHZ = 14,
};

/*
 * Set system clock prescaler
 *
 * The device is shipped with the CKDIV8 Fuse programmed - CLK_PRES_8 is set
 */
extern void sys_clk_presc(enum sys_clk_presc prescaler);

/*
 * Set system clock source, wait until stabilized
 */
extern void sys_clk_source(enum sys_clk_source source);

#endif
