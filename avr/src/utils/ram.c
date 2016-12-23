/*
 * Currently free memory left
 *
 * For license, see LICENSE.txt
 *
 * Jakub Kaderka 2016
 *
 * http://www.nongnu.org/avr-libc/user-manual/malloc.html
 */

/*
 * Return amount of currently free memory bytes
 */
int ram_free(void)
{
	extern int __bss_end, *__brkval;
	int free; //used to find current addres of top of stack

	//no data on heap
	if ((int) __brkval == 0)
		return ((int) &free) - ((int) &__bss_end);

	//something on heap
	return ((int)&free) - ((int)__brkval);
}
