EESchema Schematic File Version 2
LIBS:74xgxx
LIBS:74xx
LIBS:Altera
LIBS:ESD_Protection
LIBS:Lattice
LIBS:Oscillators
LIBS:Power_Management
LIBS:Worldsemi
LIBS:Xicor
LIBS:Zilog
LIBS:ac-dc
LIBS:actel
LIBS:adc-dac
LIBS:allegro
LIBS:analog_devices
LIBS:analog_switches
LIBS:atmel
LIBS:audio
LIBS:battery_management
LIBS:bbd
LIBS:bosch
LIBS:brooktre
LIBS:cmos4000
LIBS:cmos_ieee
LIBS:conn
LIBS:contrib
LIBS:cypress
LIBS:dc-dc
LIBS:device
LIBS:digital-audio
LIBS:diode
LIBS:display
LIBS:dsp
LIBS:elec-unifil
LIBS:ftdi
LIBS:gennum
LIBS:graphic
LIBS:hc11
LIBS:intel
LIBS:interface
LIBS:ir
LIBS:linear
LIBS:logo
LIBS:maxim
LIBS:mechanical
LIBS:memory
LIBS:microchip
LIBS:microchip_dspic33dsc
LIBS:microchip_pic10mcu
LIBS:microchip_pic12mcu
LIBS:microchip_pic16mcu
LIBS:microchip_pic18mcu
LIBS:microchip_pic32mcu
LIBS:microcontrollers
LIBS:motor_drivers
LIBS:motorola
LIBS:motors
LIBS:msp430
LIBS:nordicsemi
LIBS:nxp_armmcu
LIBS:onsemi
LIBS:opto
LIBS:philips
LIBS:power
LIBS:powerint
LIBS:pspice
LIBS:references
LIBS:regul
LIBS:relays
LIBS:rfcom
LIBS:sensors
LIBS:silabs
LIBS:siliconi
LIBS:stm8
LIBS:stm32
LIBS:supertex
LIBS:switches
LIBS:texas
LIBS:transf
LIBS:transistors
LIBS:ttl_ieee
LIBS:valves
LIBS:video
LIBS:wiznet
LIBS:xilinx
LIBS:zetex
LIBS:liion_protection-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Single cell Li-Ion battery protection"
Date "2017-03-20"
Rev "0.1"
Comp "Jakub Kaderka"
Comment1 "Over-discharge at 2.8 V"
Comment2 "Over-current at 7 Amps"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L CSD16401Q5 Q2
U 1 1 58CFFBB5
P 6300 4900
F 0 "Q2" H 6100 5200 50  0000 L CNN
F 1 "CSD16406Q3" H 6100 4650 50  0000 L CNN
F 2 "Housings_SON:VSON-8_3.3x3.3mm_Pitch0.65mm_NexFET" H 6300 5100 50  0001 C CIN
F 3 "" V 6300 4900 50  0001 L CNN
	1    6300 4900
	-1   0    0    1   
$EndComp
$Comp
L CSD16401Q5 Q1
U 1 1 58CFFBE2
P 5400 4900
F 0 "Q1" H 5200 5200 50  0000 L CNN
F 1 "CSD16406Q3" H 5200 4650 50  0000 L CNN
F 2 "Housings_SON:VSON-8_3.3x3.3mm_Pitch0.65mm_NexFET" H 5400 5100 50  0001 C CIN
F 3 "" V 5400 4900 50  0001 L CNN
	1    5400 4900
	1    0    0    1   
$EndComp
$Comp
L R R2
U 1 1 58D0041A
P 6750 4000
F 0 "R2" V 6830 4000 50  0000 C CNN
F 1 "2k2" V 6750 4000 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 6680 4000 50  0001 C CNN
F 3 "" H 6750 4000 50  0001 C CNN
	1    6750 4000
	1    0    0    1   
$EndComp
$Comp
L CONN_01X01 J1
U 1 1 58D0076B
P 4150 5100
F 0 "J1" H 4150 5200 50  0000 C CNN
F 1 "BAT-" V 4250 5100 50  0000 C CNN
F 2 "SMD_Packages:1Pin" H 4150 5100 50  0001 C CNN
F 3 "" H 4150 5100 50  0001 C CNN
	1    4150 5100
	-1   0    0    -1  
$EndComp
$Comp
L CONN_01X01 J2
U 1 1 58D00964
P 4200 2850
F 0 "J2" H 4200 2950 50  0000 C CNN
F 1 "BAT+" V 4300 2850 50  0000 C CNN
F 2 "SMD_Packages:1Pin" H 4200 2850 50  0001 C CNN
F 3 "" H 4200 2850 50  0001 C CNN
	1    4200 2850
	-1   0    0    -1  
$EndComp
$Comp
L CONN_01X01 J4
U 1 1 58D00BD9
P 7300 5100
F 0 "J4" H 7300 5200 50  0000 C CNN
F 1 "OUT-" V 7400 5100 50  0000 C CNN
F 2 "SMD_Packages:1Pin" H 7300 5100 50  0001 C CNN
F 3 "" H 7300 5100 50  0001 C CNN
	1    7300 5100
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X01 J3
U 1 1 58D00BDF
P 7250 2850
F 0 "J3" H 7250 2950 50  0000 C CNN
F 1 "OUT+" V 7350 2850 50  0000 C CNN
F 2 "SMD_Packages:1Pin" H 7250 2850 50  0001 C CNN
F 3 "" H 7250 2850 50  0001 C CNN
	1    7250 2850
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 58D00EC2
P 4550 3150
F 0 "R1" V 4630 3150 50  0000 C CNN
F 1 "330" V 4550 3150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4480 3150 50  0001 C CNN
F 3 "" H 4550 3150 50  0001 C CNN
	1    4550 3150
	-1   0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 58D00F84
P 4550 4600
F 0 "C1" H 4575 4700 50  0000 L CNN
F 1 "100n" H 4575 4500 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4588 4450 50  0001 C CNN
F 3 "" H 4550 4600 50  0001 C CNN
	1    4550 4600
	1    0    0    1   
$EndComp
Wire Wire Line
	5800 5000 5700 5000
Wire Wire Line
	5800 4900 5800 5000
Wire Wire Line
	5700 4900 5800 4900
Wire Wire Line
	5700 5100 5750 5100
Wire Wire Line
	5750 5100 5750 5000
Connection ~ 5750 5000
Wire Wire Line
	5700 4800 5750 4800
Wire Wire Line
	5750 4800 5750 4900
Connection ~ 5750 4900
Wire Wire Line
	5900 5000 6000 5000
Wire Wire Line
	5900 4900 5900 5000
Wire Wire Line
	5900 4900 6000 4900
Wire Wire Line
	6000 5100 5950 5100
Wire Wire Line
	5950 5100 5950 5000
Connection ~ 5950 5000
Wire Wire Line
	6000 4800 5950 4800
Wire Wire Line
	5950 4800 5950 4900
Connection ~ 5950 4900
Wire Wire Line
	5900 4950 5800 4950
Connection ~ 5800 4950
Connection ~ 5900 4950
Wire Wire Line
	6600 5100 7100 5100
Wire Wire Line
	6750 4150 6750 5100
Wire Wire Line
	6750 4900 6600 4900
Wire Wire Line
	5950 4350 5950 4450
Wire Wire Line
	5950 4450 6700 4450
Wire Wire Line
	6700 4450 6700 4800
Wire Wire Line
	6700 4800 6600 4800
Wire Wire Line
	5750 4350 5750 4450
Wire Wire Line
	5750 4450 5000 4450
Wire Wire Line
	5000 4450 5000 4800
Wire Wire Line
	5000 4800 5100 4800
Connection ~ 6750 4900
Wire Wire Line
	6750 3850 6750 3500
Wire Wire Line
	6750 3500 5950 3500
Wire Wire Line
	5950 3500 5950 3550
Wire Wire Line
	5750 3500 5750 3550
Wire Wire Line
	4900 4900 5100 4900
Wire Wire Line
	4900 5000 5100 5000
Connection ~ 4900 4900
Wire Wire Line
	4350 5100 5100 5100
Connection ~ 4900 5000
Connection ~ 4900 5100
$Comp
L BQ297xy U1
U 1 1 58EBA7D1
P 5850 3950
F 0 "U1" H 5600 4200 50  0000 C CNN
F 1 "BQ29700DSER" H 5950 4200 50  0000 C CNN
F 2 "Housings_SON:WSON6_1.5x1.5mm_Pitch0.5mm" H 5850 4300 50  0001 C CNN
F 3 "" H 5600 4150 50  0001 C CNN
	1    5850 3950
	0    1    1    0   
$EndComp
Wire Wire Line
	5550 3950 4900 3950
Wire Wire Line
	4400 2850 7050 2850
Wire Wire Line
	4550 3000 4550 2850
Connection ~ 4550 2850
Wire Wire Line
	4550 3300 4550 4450
Wire Wire Line
	4550 4750 4550 5100
Connection ~ 4550 5100
Wire Wire Line
	5750 3500 4550 3500
Connection ~ 4550 3500
Wire Wire Line
	4900 3950 4900 5100
$Comp
L CONN_01X02 J5
U 1 1 58F635E0
P 7000 4150
F 0 "J5" H 7000 4300 50  0000 C CNN
F 1 "NTC" V 7100 4150 50  0000 C CNN
F 2 "Connectors:GS2" H 7000 4150 50  0001 C CNN
F 3 "" H 7000 4150 50  0001 C CNN
	1    7000 4150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6950 4350 6950 5100
Wire Wire Line
	6600 5000 6750 5000
Connection ~ 6750 5000
Connection ~ 6750 5100
Connection ~ 6950 5100
$Comp
L CONN_01X01 J6
U 1 1 58F358C7
P 7300 4500
F 0 "J6" H 7300 4600 50  0000 C CNN
F 1 "TEMP" V 7400 4500 50  0000 C CNN
F 2 "SMD_Packages:1Pin" H 7300 4500 50  0001 C CNN
F 3 "" H 7300 4500 50  0001 C CNN
	1    7300 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 4350 7050 4500
Wire Wire Line
	7050 4500 7100 4500
$EndSCHEMATC
