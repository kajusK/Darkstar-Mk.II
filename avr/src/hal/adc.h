/*
 * ADC module
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#ifndef __ADC_H_
#define __ADC_H_

#include <inttypes.h>

enum ad_ref {
	AD_REF_VCC = 0x00,
	AD_REF_1_1V = 0x20,
	AD_REF_2_2V = 0x40,
	AD_REF_4_096V = 0x60,
	AD_REF_AREF = 0x80,
};

/*
 * Initialize (enable adc)
 */
extern void adc_init(void);
#define adc_enable() adc_init()

extern void adc_disable(void);

/*
 * Disable pin digital input - reduces power if analog signal is present
 */
extern void adc_pin_set(uint8_t pin);

/*
 * Enable pin digital input
 */
extern void adc_pin_unset(uint8_t pin);

/*
 * Get 10 bit ADC value
 *
 * Port is ADCx number, corresponding pin should be input and adc_pin_set should
 * be used before reading
 */
extern uint16_t adc_read(uint8_t port, enum ad_ref ref);

/*
 * Get 8 bit ADC value
 *
 * Port is ADCx number, corresponding pin should be input and adc_pin_set should
 * be used before reading
 */
extern uint8_t adc_read8(uint8_t port, enum ad_ref ref);

/*
 * Get avr core temperature in degrees Celsius
 */
extern int8_t adc_core_temp(void);

/*
 * Get supply voltage in mV
 */
extern uint16_t adc_read_vcc(void);

#endif
