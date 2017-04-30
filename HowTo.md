Howto
=====

Writing custom mode
-------------------
When the mcu is powered on, it initializes itself and run init(). This function
has to be implemented in user mode and must return.

Then the infinite loop is entered. In the loop, library reads buttons, sets the
PWM generators, and runs loop() function from user mode. The loop() must return
withing 10ms and cannot be blocking.

For example how to write custom modes browse the [simple mode](./avr/src/modes/simple.c)
and api headers [system.h](./avr/src/library/system.h), [buttons.h](./avr/src/library/buttons.h)
and [light.h](./avr/src/library/light.h).

Compilation
-----------
To compile specific mode, just run

```
make MODE=modename
```

where modename is e.g. ***simple*** or ***caving***

MCU programming
---------------
Due to limited space, there is no dedicated programming connector on the PCB.
To burn the bootloader, you'll need program the MCU before soldering or
solder programmer wires directly to mcu pins or make custom programming head
using pogo pins.

The [optioboot](https://github.com/SpenceKonde/arduino-tiny-841/tree/master/avr/bootloaders/optiboot)
bootloader is recommended way to programm the MCU. The attiny841 has no boot
section, so the optiboot creates virtual boot partition by replacing
first instruction by jump to itself.

To make programming working, SELFPRGEN fuse needs to be changed, e.g. by command:
```
avrdude -c usbtiny -p attiny841 -U efuse:w:0xFE:m
```

When the bootloader is burned, the MCU can be programmed over serial port.
When using serial interface, use supply voltage below 4 volts. On higher
voltages the internal oscillator runs faster and interferes with baudrate settings.
If you need to use UART on voltages over 4 volts, oscillator calibration might be
needed. The makefile contains programming target, just HW reset the MCU and run:

```
make MODE=modename program
```
