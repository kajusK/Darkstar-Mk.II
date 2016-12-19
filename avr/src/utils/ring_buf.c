/*
 * Circular buffer
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#include "utils/ring_buf.h"

void ring_init(ring_buf *ring, char *buffer, uint8_t size)
{
	ring->buffer = buffer;
	ring->length = size;
	ring->start = ring->end = 0;
}

uint8_t ring_push(ring_buf *ring, char data)
{
	if (ring_full(ring))
		return -1;

	ring->buffer[ring->end] = data;

	ring->end++;
	if (ring->end >= ring->length)
		ring->end = 0;

	return 0;
}

char ring_pop(ring_buf *ring)
{
	uint8_t data;

	if (ring_empty(ring))
		return -1;

	data = ring->buffer[ring->start];

	ring->start++;
	if (ring->start >= ring->length)
		ring->start = 0;

	return data;
}

uint8_t ring_full(ring_buf *ring)
{
	uint8_t next;
	next = ring->end + 1;
	if (next >= ring->length)
		next = 0;
	if (next == ring->start)
		return 1;
	return 0;
}

uint8_t ring_empty(ring_buf *ring)
{
	return ring->start == ring->end;
}
