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
L mighty_esp32_38_pins-custom_library:esp32_38_pins-custom_library U?
U 1 1 603819DF
P 5750 1950
F 0 "U?" H 6100 2597 50  0000 C CNN
F 1 "esp32_38_pins-custom_library" H 6100 2506 50  0000 C CNN
F 2 "esp32_lora_v2:esp32_devkitv4_38pin" H 6100 2415 50  0000 C CNN
F 3 "https://docs.espressif.com/projects/esp-idf/en/latest/get-started/get-started-devkitc.html" H 6100 2324 50  0000 C CNN
	1    5750 1950
	1    0    0    -1  
$EndComp
Text GLabel 6800 2550 2    50   Input ~ 0
azDir
Text GLabel 6800 2650 2    50   Input ~ 0
azStp
Text GLabel 6800 2950 2    50   Input ~ 0
azEn
Text GLabel 6800 3050 2    50   Input ~ 0
altDir
Text GLabel 6800 3150 2    50   Input ~ 0
altStp
Text GLabel 6800 3250 2    50   Input ~ 0
altEn
Text GLabel 6850 2250 2    50   Input ~ 0
SDA
Text GLabel 6850 1950 2    50   Input ~ 0
SCL
Wire Wire Line
	6650 1950 6850 1950
Wire Wire Line
	6850 2250 6650 2250
Wire Wire Line
	6650 2550 6800 2550
Wire Wire Line
	6650 2650 6800 2650
Wire Wire Line
	6650 2950 6800 2950
Wire Wire Line
	6650 3050 6800 3050
Wire Wire Line
	6650 3150 6800 3150
Wire Wire Line
	6650 3250 6800 3250
$EndSCHEMATC
