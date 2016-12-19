/*
 * ADC module
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

#include "hal/adc.h"

#ifndef DIDR0
	#define DIDR0 DIDR
#endif

//prescaler to limit adc to 200kHz
#if F_CPU < 400000
	#define AD_PRESC 1 //2
#elif F_CPU < 800000
	#define AD_PRESC 2 //4
#elif F_CPU < 1600000
	#define AD_PRESC 3 //8
#elif F_CPU < 3200000
	#define AD_PRESC 4 //16
#elif F_CPU < 6400000
	#define AD_PRESC 5 //32
#elif F_CPU < 12800000
	#define AD_PRESC 6 //64
#else
	#define AD_PRESC 7 //128
#endif

#define AD_MUX_TEMP 0x0C
//after setting internal reference, 1ms is required to settle down
#define AD_MUX_INT_REF 0x0D

void adc_init(void)
{
	ADCSRA = _BV(ADEN) | AD_PRESC;
	ADCSRB = 0;
}

void adc_disable(void)
{
	ADCSRA &= ~_BV(ADEN);
}

/*
 * Disable pin digital input - reduces power if analog signal is present
 */
void adc_pin_set(uint8_t pin)
{
	if (pin < 7)
		DIDR0 |= _BV(pin);
#ifdef DIDR1
	else
		DIDR1 |= _BV(pin - 7);
#endif
}

void adc_pin_unset(uint8_t pin)
{
	if (pin < 7)
		DIDR0 &= ~_BV(pin);
#ifdef DIDR1
	else
		DIDR1 &= ~_BV(pin - 7);
#endif
}

/*
 * Get 10 bit ADC value
 *
 * Port is ADCx number, corresponding pin should be input and adc_pin_set should
 * be used before reading
 */
uint16_t adc_read(uint8_t port, enum ad_ref ref)
{
	//if switching input to internal reference, wait until it's stable
	if (port == AD_MUX_INT_REF && ADMUXA != port) {
		ADMUXA = port;
		_delay_ms(1);
	} else {
		ADMUXA = port;
	}

	//first conversion after switching reference should be discarded
	if (ref != ADMUXB) {
		ADMUXB = ref;
		ADCSRA |= _BV(ADSC);
		loop_until_bit_is_clear(ADCSRA, ADSC);
	} else {
		ADMUXB = ref;
	}

	ADCSRA |= _BV(ADSC);
	loop_until_bit_is_clear(ADCSRA, ADSC);

	return (ADCH << 8) | ADCL;
}

/*
 * Get 8 bit ADC value
 *
 * Port is ADCx number, corresponding pin should be input and adc_pin_set should
 * be used before reading
 */
uint8_t adc_read8(uint8_t port, enum ad_ref ref)
{
	return 0x00ff & (adc_read(port, ref) >> 2);
}

/*
 * Get avr core temperature in degrees Celsius
 */
int8_t adc_core_temp(void)
{
	return adc_read(AD_MUX_TEMP, AD_REF_1_1V) - 275;
}

/*
 * Get supply voltage in hundredths of volt
 */
uint16_t adc_read_vcc(void)
{
	return 110*(1024/adc_read(AD_MUX_INT_REF, AD_REF_VCC));
}
