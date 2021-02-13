EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L wemos_mini:WeMos_mini U?
U 1 1 6022B98D
P 5400 2650
F 0 "U?" H 5400 3287 60  0000 C CNN
F 1 "WeMos_mini" H 5400 3181 60  0000 C CNN
F 2 "" H 5950 1950 60  0000 C CNN
F 3 "http://www.wemos.cc/Products/d1_mini.html" H 5950 1950 60  0001 C CNN
	1    5400 2650
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 6022C354
P 4800 2250
F 0 "#PWR?" H 4800 2100 50  0001 C CNN
F 1 "+5V" H 4815 2423 50  0000 C CNN
F 2 "" H 4800 2250 50  0001 C CNN
F 3 "" H 4800 2250 50  0001 C CNN
	1    4800 2250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6022CC0A
P 4450 2400
F 0 "#PWR?" H 4450 2150 50  0001 C CNN
F 1 "GND" H 4455 2227 50  0000 C CNN
F 2 "" H 4450 2400 50  0001 C CNN
F 3 "" H 4450 2400 50  0001 C CNN
	1    4450 2400
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 6022D3E5
P 6000 2150
F 0 "#PWR?" H 6000 2000 50  0001 C CNN
F 1 "+3.3V" H 6015 2323 50  0000 C CNN
F 2 "" H 6000 2150 50  0001 C CNN
F 3 "" H 6000 2150 50  0001 C CNN
	1    6000 2150
	1    0    0    -1  
$EndComp
Text GLabel 4800 2800 0    50   Input ~ 0
SDA
Text GLabel 4800 2700 0    50   Input ~ 0
SCL
Text GLabel 4800 2600 0    50   Input ~ 0
DIR1
Text GLabel 4800 2500 0    50   Input ~ 0
DIR2
Text GLabel 6000 2800 2    50   Input ~ 0
EN1
Text GLabel 6000 2700 2    50   Input ~ 0
EN2
Text GLabel 6000 2600 2    50   Input ~ 0
PUL1
Text GLabel 6000 2500 2    50   Input ~ 0
PUL2
Wire Wire Line
	4900 2800 4800 2800
Wire Wire Line
	4900 2700 4800 2700
Wire Wire Line
	4900 2600 4800 2600
Wire Wire Line
	4900 2500 4800 2500
Wire Wire Line
	4900 2400 4450 2400
Wire Wire Line
	4900 2300 4800 2300
Wire Wire Line
	4800 2300 4800 2250
Wire Wire Line
	5900 2500 6000 2500
Wire Wire Line
	5900 2600 6000 2600
Wire Wire Line
	5900 2700 6000 2700
Wire Wire Line
	5900 2800 6000 2800
Wire Wire Line
	5900 2300 6000 2300
Wire Wire Line
	6000 2300 6000 2150
$EndSCHEMATC
