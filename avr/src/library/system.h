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

#define system_temp() adc_core_temp();

#ifdef SUPPLY_DIODE
	#define system_voltage() (adc_read_vcc() - DIODE_FORWARD_VOLTAGE);
#else
	#define system_voltage() adc_read_vcc();
#endif

#endif
