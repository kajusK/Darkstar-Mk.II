/*
 * Buttons reading
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#ifndef __BUTTONS_H_
#define __BUTTONS_H_

#include <inttypes.h>

enum button {
	BUTTON1,
	BUTTON2,
};

enum bt_state {
	BUTTON_PRESSED,		//when just_pressed is true, pressed is true too
	BUTTON_JUST_PRESSED,
	BUTTON_RELEASED,
	BUTTON_JUST_RELEASED,
};

/*
 * Call periodically to update button status
 *
 * Shortest detectable button press is two function calls (debouncing)
 * Should be called every 10 ms
 */
extern void buttons_read(void);

/*
 * Returns time the button is pressed for. If the button is released, returns
 * latest press time.
 *
 * The time is in multiplies of 10 ms (1 - 255)
 */
extern uint8_t button_pressed_time(enum button bt);

/*
 * Returns time the button is released for. If the button is pressed, returns
 * latest released time.
 *
 * The time is in multiplies of 10 ms (1 - 255)
 */
extern uint8_t button_released_time(enum button bt);

/*
 * Get current button state
 */
extern enum bt_state button_state(enum button bt);

#endif
