/*
 * PSD (PID) regulator
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2017
 */

#ifndef __PID_H_
#define __PID_H_

struct s_pid {
	uint8_t kp;
	uint8_t ki;
	uint8_t kd;

	uint8_t prev;
	int16_t sum;

	int16_t max_sum;
	int16_t max_err;
};

extern void pid_init(struct s_pid *pid, uint8_t kp, uint8_t ki, uint8_t kd);

extern uint8_t pid_get(struct s_pid *pid, uint8_t required, uint8_t current);

#endif
