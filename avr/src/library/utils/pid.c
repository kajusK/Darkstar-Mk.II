/*
 * PSD (PID) regulator
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2017
 */

#include <inttypes.h>
#include "utils/pid.h"

#define I_MAX 8192
#define PID_SCALE 256

void pid_init(struct s_pid *pid, uint8_t kp, uint8_t ki, uint8_t kd)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;

	pid->sum = 0;
	pid->prev = 0;

	pid->max_err = INT16_MAX / kp;
	pid->max_sum = I_MAX > INT16_MAX / ki ? INT16_MAX/ki : I_MAX;
}

uint8_t pid_get(struct s_pid *pid, uint8_t required, uint8_t current)
{
	int16_t error = (int16_t)required - (int16_t)current;
	int16_t p,i,d;
	int32_t ret;

	/* proportional term */
	if (error > pid->max_err)
		p = INT16_MAX;
	else if (error < -pid->max_err)
		p = INT16_MIN;
	else
		p = pid->kp * error;

	/* integrative term */
	pid->sum += error;
	if (pid->sum > pid->max_sum) {
		pid->sum = pid->max_sum;
	} else if (pid->sum < -pid->max_sum) {
		pid->sum = -pid->max_sum;
	}
	i = pid->ki * pid->sum;

	/* derivative term */
	d = pid->kd * (pid->prev - current);
	pid->prev = current;

	ret = ((int32_t)p + (int32_t)i + (int32_t)d)/PID_SCALE;
	if (ret > INT8_MAX)
		return INT8_MAX;
	if (ret < 0)
		return 0;
	return (uint8_t) ret;
}
