/*
 * pid tuning mode
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2017
 */

#include <inttypes.h>
#include <stdio.h>

#include "buttons.h"
#include "system.h"
#include "hal/uart.h"
#include "utils/pid.h"
#include "config.h"
#include "hal/adc.h"
#include "light.h"

#ifndef UART_ENABLED
	#error "UART_ENABLED not set"
#endif

uint8_t kp;
uint8_t ki;
uint8_t kd;

uint8_t enabled = 0;

struct s_pid pid;

#define LEVEL 50
#define I_MAX 8192

void init(void)
{
	kp = PID_KP;
	ki = PID_KI;
	kd = PID_KD;
	pid_init(&pid, kp, ki, kd);
	wdt_disable();
}

void loop(void)
{
	char c = 0;
	uint8_t res, adc;

	if (uart_check_rx() != 0)
		c = getchar();

	switch (c) {
		case 'w':
			kp = kp == 255 ? kp : kp+1;
			break;
		case 'q':
			kp = kp == 0 ? kp : kp-1;
			break;
		case 's':
			ki = ki == 255 ? ki : ki+1;
			break;
		case 'a':
			ki = ki == 0 ? ki : ki-1;
			break;
		case 'x':
			kd = kd == 255 ? kd : kd+1;
			break;
		case 'z':
			kd = kd == 0 ? kd : kd-1;
			break;
		case 'r':
			enabled = 1;
			break;
		case 'b':
			enabled = 0;
			break;

	}

	pid.kp = kp;
	pid.ki = ki;
	pid.kd = kd;
	pid.max_err = INT16_MAX / kp;
	pid.max_sum = I_MAX > INT16_MAX / ki ? INT16_MAX/ki : I_MAX;

	printf("\e[1;1H\e[2JKP: %d\n", kp);
	printf("KI: %d\n", ki);
	printf("KD: %d\n", kd);
	adc = adc_read8(PHOTOTRANS_ADC, AD_REF_1_1V);
	res = pid_get(&pid, LEVEL, adc);
	printf("adc: %d\n", adc);

	if (res < DEFAULT_DIM_CURRENT)
		res = DEFAULT_DIM_CURRENT;

	if (res > 150)
		res = 150;

	printf("res: %d\n", res);

	if (enabled)
		light_set_simple(LED_FLOOD, res, MODE_NORMAL);
	else
		light_set_simple(LED_FLOOD, 0, MODE_NORMAL);
}
