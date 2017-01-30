/*
 * Buttons reading
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 */

#include <avr/io.h>
#include <inttypes.h>

#include "config.h"
#include "buttons.h"

#define BT1_ON (!(PIN(BUTTON1_PORT) & _BV(BUTTON1_PIN)))
#define BT2_ON (!(PIN(BUTTON2_PORT) & _BV(BUTTON2_PIN)))

/*
 * Status keeps last 3 states of button
 * 000 - released
 * 001 - released - need 2 states for debouncing
 * 010 - released - button bounced
 * 011 - just pressed
 * 111 - pressed
 * 110 - just released
 * 100 - released
 * 101 - released
 */
#define BT_MASK 0x07
#define BT_JUST_PRESSED(status) (status == 0x03)
#define BT_PRESSED(status) (status == 0x07 || BT_JUST_PRESSED(status))
#define BT_JUST_RELEASED(status) (status == 0x06)
#define BT_RELEASED(status) (!BT_PRESSED(status) && !BT_JUST_PRESSED(status))

static uint8_t bt1_state, bt1_t_pressed, bt1_t_released;
static uint8_t bt2_state, bt2_t_pressed, bt2_t_released;

static inline enum bt_state get_state(uint8_t bt_state)
{
	if (BT_JUST_PRESSED(bt_state))
		return BUTTON_JUST_PRESSED;
	if (BT_PRESSED(bt_state))
		return BUTTON_PRESSED;
	if (BT_JUST_RELEASED(bt_state))
		return BUTTON_JUST_RELEASED;
	return BUTTON_RELEASED;
}

/*
 * Call periodically to update button status
 *
 * Shortest detectable button press is two function calls (debouncing)
 * Should be called every 10 ms
 */
void buttons_read(void)
{
	//add current state
	bt1_state = ((bt1_state << 1) | BT1_ON) & BT_MASK;

	if (BT_JUST_PRESSED(bt1_state))
		bt1_t_pressed = 1;
	else if (BT_JUST_RELEASED(bt1_state))
		bt1_t_released = 0;
	else if (BT_PRESSED(bt1_state))
		bt1_t_pressed = bt1_t_pressed == 0xff ? 0xff : bt1_t_pressed+1;
	else
		bt1_t_released = bt1_t_released == 0xff ? 0xff : bt1_t_released+1;

	//add current state
	bt2_state = ((bt2_state << 1) | BT2_ON) & BT_MASK;

	if (BT_JUST_PRESSED(bt2_state))
		bt2_t_pressed = 1;
	else if (BT_JUST_RELEASED(bt2_state))
		bt2_t_released = 0;
	else if (BT_PRESSED(bt2_state))
		bt2_t_pressed = bt2_t_pressed == 0xff ? 0xff : bt2_t_pressed+1;
	else
		bt2_t_released = bt2_t_released == 0xff ? 0xff : bt2_t_released+1;
}

/*
 * Returns time the button is pressed for. If the button is released, returns
 * latest press time.
 *
 * The time is in multiplies of 10 ms (1 - 255)
 */
uint8_t button_pressed_time(enum button bt)
{
	if (bt == BUTTON1)
		return bt1_t_pressed;
	return bt2_t_pressed;
}

/*
 * Returns time the button is released for. If the button is pressed, returns
 * latest released time.
 *
 * The time is in multiplies of 10 ms (1 - 255)
 */
uint8_t button_released_time(enum button bt)
{
	if (bt == BUTTON1)
		return bt1_t_released;
	return bt2_t_released;
}

/*
 * Get current button state
 */
enum bt_state button_state(enum button bt)
{
	if (bt == BUTTON1)
		return get_state(bt1_state);
	return get_state(bt2_state);
}
