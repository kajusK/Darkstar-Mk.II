/*
 * Circular buffer
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#ifndef __RING_BUF_H_
#define __RING_BUF_H_

#include <inttypes.h>

typedef struct {
	char *buffer;
	uint8_t length;
	volatile uint8_t start;
	volatile uint8_t end;
} ring_buf;

/*
 * Due to limited ram, buffer shoud point to already allocated memory
 * of 'size' length
 */
extern void ring_init(ring_buf *ring, char *buffer, uint8_t size);

/*
 * return 0 if suceeded, -1 otherwise
 */
extern uint8_t ring_push(ring_buf *ring, char data);

/*
 * returns -1 if empty
 */
extern char ring_pop(ring_buf *ring);

extern uint8_t ring_full(ring_buf *ring);

extern uint8_t ring_empty(ring_buf *ring);

#endif
