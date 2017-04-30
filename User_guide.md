User guide
==========

Darkstar Mk.II light has four (one optional) sources of light:

- High power spot LED
- High power flood LED
- Low power red LED
- Low power white LED (optional)

The lamp is powered by 3,7 V Li-Ion rechargeable battery. Alternative power sources
are possible, voltage of such source must fall between 3 and 5 V.
Maximal worst condition current drawn is 4 A.

The lamp itself and battery box were designed for wet caves and harsh environment.
No drying is required after immersion in water. Cleaning can be done simply by
washing the whole lamp in water. Watertightness can be guaranteed only when
sealing O-rings are clean and lubricated properly though.

Programming
===========
There are two ways to update firmware. The hard one is using AVR ISP programmer,
soldering 6 wires to PCB (or using pogo pins). Simpler way is using bootloader
(if it was uploaded already). The darkstar Mk.II is using standard optiboot
bootloader compatible with arduino IDE.

See [HowTo](./HowTo.md).

Modes
=====
Caving firmware
---------------
While using caving mode firmware, the lamp can be in one of three modes:

- Normal
- Programming
- Configuration

### Normal mode

In normal mode, short button presses are changing brightness of all LEDs
based on the programmed levels. Long press of Down button while LEDs are
on will switch between normal and automated brightness control. The selected
mode is signalled by blinking - 1 blink is normal mode, 2 blinks is automated.

If Up button is hold, programming mode is entered. It's signalized by 1 slow
blink.

If lamp is off and Down button is hold, the lock mode is activated. To disable
lock mode, just briefly push Up and Down buttons several times. If up button
is hold in off state, current battery level is reported by 1 (low) to 5 (full)
blinks.

Additionally, when the battery drops to ~50 %, it's signalized by one blink.
Low battery is 3 blinks. Something in the middle is signalized by 2 blinks. After
the blinking is finished, output power is limited by amount based on remaining
charge. Output power can be also limited when body temperature limit is reached.

### Programming mode

In this mode, current level selected can be changed for each LED. Up button
cycles SPOT LED levels, Down is for FLOOD. Holding Down toggles RED LED.

Programming mode is left automatically after 2,5 seconds of inactivity. Return
is followed by 1 slow blink.

### Configuration mode

Configuration mode is entered by holding Down button while connecting batteries.
Once in configuration mode, the red LED is on.

Up/Down buttons change number of modes available in normal mode. Long Down press
will blink out number of modes enabled.

Long Up press enables/disables programming mode. Enabled programming is confirmed by
two fast blinks. Disabled by one.

After the configuration is finished, wait for 2 seconds, disconnect the battery
and connect it again.


Simple firmware
---------------
Quite simple firmware with almost no features. It's more like technology
demonstration.

Holding DOWN button toggles RED LED. Holding UP button powers the headlamp
on and off. Short Up press cycles through spot LED power levels, Down press
changes flood LED output power.

There's no warning about battery state and heat/voltage limits are only for
the critical values (extra hot and almost dead battery). It won't let you in
the darkness immediately though, the hardcoded limits set output to minimum first.

Safety
======

- Always carry a second, independent source of light during your caving trip!

- Laser class 2 device. Do not look directly into the light.

- Keep the lamp and batteries away from children.

- Never short or mechanically damage the batteries, Li-Ion cell might catch fire or explode

- Never left batteries connected to charger without supervision

- When using alternate batteries, make sure the voltage applied and polarity is correct
