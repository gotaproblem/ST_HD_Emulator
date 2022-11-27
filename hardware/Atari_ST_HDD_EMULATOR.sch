EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "ATARI ACSI HARD DRIVE CONTROLLER EMULATOR"
Date "2022-11-01"
Rev "2.0"
Comp "BBAN"
Comment1 "Author: Steve Bradford"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L 74xx:74LS125 U4
U 1 1 632CB585
P 1400 4150
F 0 "U4" H 1400 4467 50  0000 C CNN
F 1 "74HCT125PW" H 1400 4376 50  0000 C CNN
F 2 "Package_SO:TSSOP-14_4.4x5mm_P0.65mm" H 1400 4150 50  0001 C CNN
F 3 "" H 1400 4150 50  0001 C CNN
	1    1400 4150
	-1   0    0    -1  
$EndComp
$Comp
L 74xx:74LS125 U4
U 2 1 632CC46D
P 1400 3200
F 0 "U4" H 1400 3517 50  0000 C CNN
F 1 "74HCT125PW" H 1400 3426 50  0000 C CNN
F 2 "Package_SO:TSSOP-14_4.4x5mm_P0.65mm" H 1400 3200 50  0001 C CNN
F 3 "" H 1400 3200 50  0001 C CNN
	2    1400 3200
	-1   0    0    -1  
$EndComp
Text Notes 4050 1250 0    50   ~ 0
Data bus
Text Notes 4050 3600 0    50   ~ 0
Control Bus
Wire Wire Line
	5000 1850 6050 1850
$Comp
L power:GND #PWR0104
U 1 1 632E0BE3
P 3550 2550
F 0 "#PWR0104" H 3550 2300 50  0001 C CNN
F 1 "GND" H 3555 2377 50  0000 C CNN
F 2 "" H 3550 2550 50  0001 C CNN
F 3 "" H 3550 2550 50  0001 C CNN
	1    3550 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 4150 1700 4400
Wire Wire Line
	1700 4400 1400 4400
Wire Wire Line
	1700 3200 1700 3450
Wire Wire Line
	1700 3450 1400 3450
$Comp
L 74xx:74LS125 U4
U 5 1 632E7921
P 6650 5250
F 0 "U4" H 6880 5296 50  0000 L CNN
F 1 "74HCT125PW" H 6880 5205 50  0000 L CNN
F 2 "Package_SO:TSSOP-14_4.4x5mm_P0.65mm" H 6650 5250 50  0001 C CNN
F 3 "" H 6650 5250 50  0001 C CNN
	5    6650 5250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 632E8960
P 6650 5750
F 0 "#PWR0106" H 6650 5500 50  0001 C CNN
F 1 "GND" H 6655 5577 50  0000 C CNN
F 2 "" H 6650 5750 50  0001 C CNN
F 3 "" H 6650 5750 50  0001 C CNN
	1    6650 5750
	1    0    0    -1  
$EndComp
Text GLabel 6050 3350 0    50   Output ~ 0
DRQ
Text GLabel 6050 2650 0    50   Output ~ 0
IRQ
Wire Wire Line
	1700 4150 1800 4150
Connection ~ 1700 4150
Wire Wire Line
	1700 3200 1800 3200
Connection ~ 1700 3200
Text Label 4150 1550 2    50   ~ 0
D0
Text Label 4150 1650 2    50   ~ 0
D1
Text Label 4150 1750 2    50   ~ 0
D2
Text Label 4150 1850 2    50   ~ 0
D3
Text Label 4150 1950 2    50   ~ 0
D4
Text Label 4150 2050 2    50   ~ 0
D5
Text Label 4150 2150 2    50   ~ 0
D6
Text Label 4150 2250 2    50   ~ 0
D7
Text GLabel 6050 2550 0    50   Input ~ 0
CS
Text GLabel 6050 2850 0    50   Input ~ 0
RST
Text GLabel 6050 2950 0    50   Input ~ 0
ACK
Text GLabel 6050 3050 0    50   Input ~ 0
A1
Text GLabel 6050 3150 0    50   Input ~ 0
RW
Text GLabel 3950 3850 2    50   Output ~ 0
CS
Text GLabel 3950 3950 2    50   Output ~ 0
RST
Text GLabel 3950 4050 2    50   Output ~ 0
ACK
Text GLabel 3950 4150 2    50   Output ~ 0
A1
Text GLabel 3950 4250 2    50   Output ~ 0
RW
Wire Wire Line
	1050 1550 1450 1550
Wire Wire Line
	1450 1750 1050 1750
Wire Wire Line
	1450 1950 1050 1950
Wire Wire Line
	1450 2150 1050 2150
Text GLabel 3150 4150 0    50   Input ~ 0
AA1
Text GLabel 3150 4050 0    50   Input ~ 0
AACK
Text GLabel 3150 3950 0    50   Input ~ 0
ARST
Text GLabel 1450 2050 0    50   Output ~ 0
AA1
Text GLabel 1450 1850 0    50   Output ~ 0
AACK
Text GLabel 1450 1650 0    50   Output ~ 0
ARST
Text GLabel 7450 3450 2    50   Input ~ 0
SPI_RX
Text GLabel 7450 3150 2    50   Output ~ 0
SPI_CLK
Text GLabel 7450 3050 2    50   Output ~ 0
SPI_TX
Text GLabel 9700 2300 0    50   Input ~ 0
SPI_CLK
Text GLabel 9700 2500 0    50   Output ~ 0
SPI_RX
Text GLabel 9700 2100 0    50   Input ~ 0
SPI_TX
$Comp
L Device:LED_Small D1
U 1 1 63388220
P 9100 1300
F 0 "D1" V 9146 1230 50  0000 R CNN
F 1 "LED" V 9055 1230 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric" V 9100 1300 50  0001 C CNN
F 3 "~" V 9100 1300 50  0001 C CNN
	1    9100 1300
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small R1
U 1 1 6338B9F0
P 9100 1500
F 0 "R1" H 9159 1546 50  0000 L CNN
F 1 "R220" H 9159 1455 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 9100 1500 50  0001 C CNN
F 3 "~" H 9100 1500 50  0001 C CNN
	1    9100 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	9100 1600 9100 1700
Text Notes 8500 800  0    50   ~ 0
LED1,2\nOrange\nfwdV 2.2v @ 5mA
$Comp
L Device:R_Small R2
U 1 1 633977F8
P 8650 1500
F 0 "R2" H 8709 1546 50  0000 L CNN
F 1 "R220" H 8709 1455 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 8650 1500 50  0001 C CNN
F 3 "~" H 8650 1500 50  0001 C CNN
	1    8650 1500
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_Small D2
U 1 1 633994DB
P 8650 1300
F 0 "D2" V 8696 1230 50  0000 R CNN
F 1 "LED" V 8605 1230 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric" V 8650 1300 50  0001 C CNN
F 3 "~" V 8650 1300 50  0001 C CNN
	1    8650 1300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8650 1600 8650 1700
$Comp
L Device:C C1
U 1 1 633AA233
P 9100 5400
F 0 "C1" H 9215 5446 50  0000 L CNN
F 1 "100nF" H 9215 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9138 5250 50  0001 C CNN
F 3 "~" H 9100 5400 50  0001 C CNN
	1    9100 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 633AA52F
P 9750 5400
F 0 "C2" H 9865 5446 50  0000 L CNN
F 1 "100nF" H 9865 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9788 5250 50  0001 C CNN
F 3 "~" H 9750 5400 50  0001 C CNN
	1    9750 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 633AAC68
P 10250 5400
F 0 "C3" H 10365 5446 50  0000 L CNN
F 1 "100nF" H 10365 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 10288 5250 50  0001 C CNN
F 3 "~" H 10250 5400 50  0001 C CNN
	1    10250 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	9100 5550 9550 5550
Connection ~ 9750 5550
$Comp
L power:GND #PWR0112
U 1 1 633AF98C
P 9550 5550
F 0 "#PWR0112" H 9550 5300 50  0001 C CNN
F 1 "GND" H 9555 5377 50  0000 C CNN
F 2 "" H 9550 5550 50  0001 C CNN
F 3 "" H 9550 5550 50  0001 C CNN
	1    9550 5550
	1    0    0    -1  
$EndComp
Connection ~ 9550 5550
Wire Wire Line
	9550 5550 9750 5550
Text Notes 8250 4950 0    50   ~ 0
Power\nPI PICO provides the 5v and 3.3v for the circuit via its USB C connection\nTotal current draw must not excede 300mA
Wire Wire Line
	9750 5550 10250 5550
Text Notes 9800 4350 0    50   ~ 0
SPI_TX MOSI (Master Out Slave In)\nSPI_RX MISO (Master In Slave Out)
Text Notes 10350 3450 0    50   ~ 0
DM3AT-SF-PEJM5\nSPI MODE\n\n1 NC\n2 CARD SELECT\n3 DI MOSI\n4 3.3v\n5 SCLK\n6 GND\n7 DO MISO\n8 NC\n9 CARD DETECT
Text Notes 850  1100 0    50   ~ 0
ACSI Bus
Text Notes 850  1200 0    50   ~ 0
Atari Computer Systems Interface
Text GLabel 3150 3850 0    50   Input ~ 0
ACS
Text GLabel 1950 2350 2    50   Output ~ 0
ACS
Text GLabel 1450 2250 0    50   Output ~ 0
ARW
Text GLabel 3150 4250 0    50   Input ~ 0
ARW
Text Label 1950 1550 0    50   ~ 0
AD0
Wire Wire Line
	1950 1950 3150 1950
Wire Wire Line
	3150 1850 1950 1850
Wire Wire Line
	1950 1750 3150 1750
Wire Wire Line
	3150 1650 1950 1650
Wire Wire Line
	1950 1550 3150 1550
$Comp
L Connector_Generic:Conn_02x10_Top_Bottom J1
U 1 1 632CDC63
P 1750 1950
F 0 "J1" H 1800 2567 50  0000 C CNN
F 1 "IDC 20pin" H 1800 2476 50  0000 C CNN
F 2 "Connector_IDC:IDC-Header_2x10_P2.54mm_Vertical" H 1750 1950 50  0001 C CNN
F 3 "~" H 1750 1950 50  0001 C CNN
	1    1750 1950
	-1   0    0    -1  
$EndComp
Text Label 1950 1650 0    50   ~ 0
AD1
Text Label 1950 1750 0    50   ~ 0
AD2
Text Label 1950 1850 0    50   ~ 0
AD3
Text Label 1950 1950 0    50   ~ 0
AD4
$Comp
L Logic_LevelTranslator:TXB0108DQSR U2
U 1 1 635E61D3
P 3550 1850
F 0 "U2" H 3550 3000 50  0000 C CNN
F 1 "TXB0108PWR" H 3550 2900 50  0000 C CNN
F 2 "Package_SO:TSSOP-20_4.4x6.5mm_P0.65mm" H 3550 1100 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/txb0108.pdf" H 3550 1750 50  0001 C CNN
	1    3550 1850
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3950 1550 6050 1550
Wire Wire Line
	3950 1650 6050 1650
Wire Wire Line
	5000 1850 5000 1750
Wire Wire Line
	3950 1750 5000 1750
Wire Wire Line
	4800 2050 4800 1950
Wire Wire Line
	4800 2050 6050 2050
Wire Wire Line
	3950 1950 4800 1950
Wire Wire Line
	4700 2150 4700 2050
Wire Wire Line
	4700 2150 6050 2150
Wire Wire Line
	3950 2050 4700 2050
Wire Wire Line
	4600 2350 4600 2150
Wire Wire Line
	4600 2350 6050 2350
Wire Wire Line
	3950 2150 4600 2150
Wire Wire Line
	4500 2450 4500 2250
Wire Wire Line
	4500 2450 6050 2450
Wire Wire Line
	3950 2250 4500 2250
Text GLabel 6050 3450 0    50   Output ~ 0
DATABUSENABLE
Text GLabel 3950 1450 2    50   Input ~ 0
DATABUSENABLE
$Comp
L Logic_LevelTranslator:TXB0108DQSR U3
U 1 1 636735D5
P 3550 4150
F 0 "U3" H 3550 5300 50  0000 C CNN
F 1 "TXB0108PWR" H 3550 5200 50  0000 C CNN
F 2 "Package_SO:TSSOP-20_4.4x6.5mm_P0.65mm" H 3550 3400 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/txb0108.pdf" H 3550 4050 50  0001 C CNN
	1    3550 4150
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR0119
U 1 1 6367E0A6
P 3550 4850
F 0 "#PWR0119" H 3550 4600 50  0001 C CNN
F 1 "GND" H 3555 4677 50  0000 C CNN
F 2 "" H 3550 4850 50  0001 C CNN
F 3 "" H 3550 4850 50  0001 C CNN
	1    3550 4850
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS85 U5
U 1 1 6369A9D9
P 3500 6500
F 0 "U5" H 3100 7300 50  0000 C CNN
F 1 "74HCT85PW" H 3200 7200 50  0000 C CNN
F 2 "Package_SO:TSSOP-16_4.4x5mm_P0.65mm" H 3500 6500 50  0001 C CNN
F 3 "" H 3500 6500 50  0001 C CNN
	1    3500 6500
	1    0    0    -1  
$EndComp
Text Notes 4000 7050 0    50   ~ 0
A=B high Controller selected
Wire Wire Line
	1950 7550 2200 7550
Wire Wire Line
	2250 7250 2250 7550
Text GLabel 4000 6900 2    50   Output ~ 0
CONTROLLERID
Text GLabel 3000 6100 0    50   Input ~ 0
AA1
Text GLabel 3000 6400 0    50   Input ~ 0
AD5
Text GLabel 3000 6300 0    50   Input ~ 0
AD6
Text GLabel 3000 6200 0    50   Input ~ 0
AD7
$Comp
L power:GND #PWR0120
U 1 1 636C35F1
P 3500 7200
F 0 "#PWR0120" H 3500 6950 50  0001 C CNN
F 1 "GND" H 3505 7027 50  0000 C CNN
F 2 "" H 3500 7200 50  0001 C CNN
F 3 "" H 3500 7200 50  0001 C CNN
	1    3500 7200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0121
U 1 1 636C3A36
P 2200 7550
F 0 "#PWR0121" H 2200 7300 50  0001 C CNN
F 1 "GND" H 2205 7377 50  0000 C CNN
F 2 "" H 2200 7550 50  0001 C CNN
F 3 "" H 2200 7550 50  0001 C CNN
	1    2200 7550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 6800 2250 6800
Wire Wire Line
	3000 6900 1950 6900
Wire Wire Line
	2250 7050 2250 6800
Connection ~ 2250 6800
Wire Wire Line
	2250 6800 1950 6800
$Comp
L Device:R_Small R17
U 1 1 636AE2AE
P 2250 7150
F 0 "R17" H 2050 7100 50  0000 L CNN
F 1 "R10K" H 2000 7000 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 2250 7150 50  0001 C CNN
F 3 "~" H 2250 7150 50  0001 C CNN
	1    2250 7150
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R16
U 1 1 636AD024
P 1950 7150
F 0 "R16" H 1750 7100 50  0000 L CNN
F 1 "R10K" H 1700 7000 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 1950 7150 50  0001 C CNN
F 3 "~" H 1950 7150 50  0001 C CNN
	1    1950 7150
	1    0    0    -1  
$EndComp
Text Notes 950  5950 0    50   ~ 0
ID 0  1-2 OFF, 3-4 OFF, 5-6 OFF\nID 1  1-2 ON,  3-4 OFF, 5-6 OFF\nID 2  1-2 OFF, 3-4 ON,  5-6 OFF\nID 3  1-2 ON,  3-4 ON,  5-6 OFF\nID 4  1-2 OFF, 2-3 OFF, 5-6 ON\nID 5  1-2 ON,  2-3 OFF, 5-6 ON\nID 6  1-2 OFF, 2-3 ON,  5-6 ON\nID 7  NOT USED
Text GLabel 3150 4550 0    50   Input ~ 0
CONTROLLERID
Text GLabel 3950 4550 2    50   Output ~ 0
SELECTED
Text GLabel 7450 2450 2    50   Input ~ 0
SELECTED
NoConn ~ 4000 6100
NoConn ~ 4000 6200
NoConn ~ 4000 6300
NoConn ~ 4000 6700
NoConn ~ 4000 6800
NoConn ~ 7450 2050
NoConn ~ 7450 1850
NoConn ~ 7450 1650
Text GLabel 1950 2050 2    50   BiDi ~ 0
AD5
Text GLabel 1950 2150 2    50   BiDi ~ 0
AD6
Text GLabel 1950 2250 2    50   BiDi ~ 0
AD7
Wire Wire Line
	4900 1850 4900 1950
Wire Wire Line
	4900 1950 6050 1950
Wire Wire Line
	3950 1850 4900 1850
Text GLabel 7450 2850 2    50   Input ~ 0
CD0
Text GLabel 7450 2650 2    50   Input ~ 0
CD1
Text GLabel 9700 2700 0    50   Output ~ 0
CD1
NoConn ~ 9700 1900
NoConn ~ 9700 2600
Text GLabel 9700 2000 0    50   Input ~ 0
CS1
Text GLabel 7450 2950 2    50   Output ~ 0
CS1
Text GLabel 7450 3350 2    50   Output ~ 0
CS0
$Comp
L power:GND #PWR0109
U 1 1 636C8DE6
P 9200 2400
F 0 "#PWR0109" H 9200 2150 50  0001 C CNN
F 1 "GND" H 9200 2250 50  0000 C CNN
F 2 "" H 9200 2400 50  0001 C CNN
F 3 "" H 9200 2400 50  0001 C CNN
	1    9200 2400
	1    0    0    -1  
$EndComp
Text GLabel 9700 3600 0    50   Input ~ 0
SPI_CLK
Text GLabel 9700 3800 0    50   Output ~ 0
SPI_RX
Text GLabel 9700 3400 0    50   Input ~ 0
SPI_TX
Text GLabel 9700 4000 0    50   Output ~ 0
CD0
NoConn ~ 9700 3200
NoConn ~ 9700 3900
Text GLabel 9700 3300 0    50   Input ~ 0
CS0
$Comp
L Connector_Generic:Conn_01x09 J2
U 1 1 636C8E02
P 9900 3600
F 0 "J2" H 9980 3642 50  0000 L CNN
F 1 "uSD Card 0" H 9980 3551 50  0000 L CNN
F 2 "Connector_Card:microSD_HC_Hirose_DM3AT-SF-PEJM5" H 9900 3600 50  0001 C CNN
F 3 "~" H 9900 3600 50  0001 C CNN
	1    9900 3600
	1    0    0    -1  
$EndComp
Text GLabel 8650 1700 0    50   Input ~ 0
CS1
Text GLabel 9100 1700 0    50   Input ~ 0
CS0
Text GLabel 3150 2050 0    50   Input ~ 0
AD5
Text GLabel 3150 2150 0    50   Input ~ 0
AD6
Text GLabel 3150 2250 0    50   Input ~ 0
AD7
$Comp
L Device:R_Small R3
U 1 1 636D74DA
P 5000 1000
F 0 "R3" H 4800 1050 50  0000 L CNN
F 1 "R1K" H 4800 950 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5000 1000 50  0001 C CNN
F 3 "~" H 5000 1000 50  0001 C CNN
	1    5000 1000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0115
U 1 1 636D80B3
P 5000 1100
F 0 "#PWR0115" H 5000 850 50  0001 C CNN
F 1 "GND" H 5005 927 50  0000 C CNN
F 2 "" H 5000 1100 50  0001 C CNN
F 3 "" H 5000 1100 50  0001 C CNN
	1    5000 1100
	1    0    0    -1  
$EndComp
Text GLabel 5000 850  2    50   Input ~ 0
DATABUSENABLE
Text Notes 5000 750  0    50   ~ 0
Make sure DATABUSENABLE is\nlow on power-up
Wire Wire Line
	5000 850  5000 900 
Text Notes 2950 5300 0    50   ~ 10
NOT implemented yet
$Comp
L Device:R_Small R4
U 1 1 6371F668
P 1800 2950
F 0 "R4" H 1859 2996 50  0000 L CNN
F 1 "R1K" H 1859 2905 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 1800 2950 50  0001 C CNN
F 3 "~" H 1800 2950 50  0001 C CNN
	1    1800 2950
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R5
U 1 1 63722F33
P 1800 3900
F 0 "R5" H 1859 3946 50  0000 L CNN
F 1 "R1K" H 1859 3855 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 1800 3900 50  0001 C CNN
F 3 "~" H 1800 3900 50  0001 C CNN
	1    1800 3900
	1    0    0    -1  
$EndComp
Text GLabel 1800 3200 2    50   Input ~ 0
DRQ
Text GLabel 1800 4150 2    50   Input ~ 0
IRQ
Wire Wire Line
	1800 3200 1800 3050
Wire Wire Line
	1800 4000 1800 4150
Text Notes 1100 4650 0    50   ~ 0
Make sure DRQ & IRQ are high\non power-up and power-down
$Comp
L Connector_Generic:Conn_01x09 J3
U 1 1 6369F92E
P 9900 2300
F 0 "J3" H 9980 2342 50  0000 L CNN
F 1 "uSD Card 1" H 9980 2251 50  0000 L CNN
F 2 "Connector_Card:microSD_HC_Hirose_DM3AT-SF-PEJM5" H 9900 2300 50  0001 C CNN
F 3 "~" H 9900 2300 50  0001 C CNN
	1    9900 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9700 2400 9200 2400
$Comp
L power:GND #PWR0108
U 1 1 63860839
P 9250 3700
F 0 "#PWR0108" H 9250 3450 50  0001 C CNN
F 1 "GND" H 9250 3550 50  0000 C CNN
F 2 "" H 9250 3700 50  0001 C CNN
F 3 "" H 9250 3700 50  0001 C CNN
	1    9250 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	9700 3700 9250 3700
Text GLabel 1950 2450 2    50   Input ~ 0
AIRQ
Text GLabel 1450 2350 0    50   Input ~ 0
ADRQ
Text GLabel 1100 4150 0    50   Output ~ 0
AIRQ
Text GLabel 1100 3200 0    50   Output ~ 0
ADRQ
$Comp
L Device:C C5
U 1 1 638FD3DD
P 8800 2300
F 0 "C5" H 8915 2346 50  0000 L CNN
F 1 "1nF" H 8915 2255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8838 2150 50  0001 C CNN
F 3 "~" H 8800 2300 50  0001 C CNN
	1    8800 2300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C6
U 1 1 638FE426
P 8850 3600
F 0 "C6" H 8965 3646 50  0000 L CNN
F 1 "1nF" H 8965 3555 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8888 3450 50  0001 C CNN
F 3 "~" H 8850 3600 50  0001 C CNN
	1    8850 3600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0129
U 1 1 638FEA79
P 8800 2450
F 0 "#PWR0129" H 8800 2200 50  0001 C CNN
F 1 "GND" H 8800 2300 50  0000 C CNN
F 2 "" H 8800 2450 50  0001 C CNN
F 3 "" H 8800 2450 50  0001 C CNN
	1    8800 2450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0130
U 1 1 638FEE58
P 8850 3750
F 0 "#PWR0130" H 8850 3500 50  0001 C CNN
F 1 "GND" H 8850 3600 50  0000 C CNN
F 2 "" H 8850 3750 50  0001 C CNN
F 3 "" H 8850 3750 50  0001 C CNN
	1    8850 3750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW1
U 1 1 63911F30
P 5300 5950
F 0 "SW1" H 5300 6235 50  0000 C CNN
F 1 "PICO RESET" H 5300 6144 50  0000 C CNN
F 2 "Button_Switch_SMD:SW_SPST_B3U-1000P" H 5300 6150 50  0001 C CNN
F 3 "~" H 5300 6150 50  0001 C CNN
	1    5300 5950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0138
U 1 1 6391244F
P 5100 5950
F 0 "#PWR0138" H 5100 5700 50  0001 C CNN
F 1 "GND" H 5105 5777 50  0000 C CNN
F 2 "" H 5100 5950 50  0001 C CNN
F 3 "" H 5100 5950 50  0001 C CNN
	1    5100 5950
	1    0    0    -1  
$EndComp
Text GLabel 7450 2550 2    50   Input ~ 0
PICO_RST
Text GLabel 5500 5950 2    50   Output ~ 0
PICO_RST
$Comp
L Device:C C4
U 1 1 63917611
P 10750 5400
F 0 "C4" H 10865 5446 50  0000 L CNN
F 1 "100nF" H 10865 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 10788 5250 50  0001 C CNN
F 3 "~" H 10750 5400 50  0001 C CNN
	1    10750 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	10750 5550 10250 5550
Connection ~ 10250 5550
Text Notes 950  5300 0    50   ~ 10
CONTROLLER ID
Text Notes 5450 7300 0    50   ~ 10
RTC ENABLE
Text Notes 5450 7400 0    50   ~ 0
1-2  ON
Connection ~ 2200 7550
Wire Wire Line
	2200 7550 2250 7550
Text GLabel 3000 6600 0    50   Input ~ 0
AA1
Text GLabel 7450 2350 2    50   Input ~ 0
RTC_ENABLE
Wire Notes Line
	750  5150 750  7800
Wire Notes Line
	6050 7800 6050 5150
Wire Notes Line
	6050 5150 750  5150
NoConn ~ 1450 2450
Text Label 7650 1550 0    50   ~ 10
VS_5V
Wire Wire Line
	7650 1550 7450 1550
Text Label 7650 1950 0    50   ~ 10
VS_3V3
Text Label 9850 6150 0    50   ~ 10
VS_5V
Wire Wire Line
	9850 6150 9850 6050
Text Label 10300 6150 0    50   ~ 10
VS_3V3
Wire Wire Line
	10300 6150 10300 6050
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 63974313
P 10750 6050
F 0 "#FLG0103" H 10750 6125 50  0001 C CNN
F 1 "PWR_FLAG" H 10750 6223 50  0000 C CNN
F 2 "" H 10750 6050 50  0001 C CNN
F 3 "~" H 10750 6050 50  0001 C CNN
	1    10750 6050
	1    0    0    -1  
$EndComp
Text Label 10750 6150 0    50   ~ 10
GND
Wire Wire Line
	10750 6150 10750 6050
Text Label 9100 5250 0    50   ~ 10
VS_5V
Text Label 6650 4750 0    50   ~ 10
VS_5V
Text Label 3450 3450 2    50   ~ 10
VS_5V
Text Label 3450 1150 2    50   ~ 10
VS_5V
Text Label 7650 1750 0    50   ~ 10
GND
Wire Wire Line
	7450 1750 7650 1750
Wire Wire Line
	7450 1950 7650 1950
Text Label 10750 5250 0    50   ~ 10
VS_3V3
Text Label 10250 5250 0    50   ~ 10
VS_3V3
Text Label 3650 3450 0    50   ~ 10
VS_3V3
Text Label 3650 1150 0    50   ~ 10
VS_3V3
Text Label 1800 2850 0    50   ~ 10
VS_3V3
Text Label 1800 3800 0    50   ~ 10
VS_3V3
Text Label 8650 1200 0    50   ~ 10
VS_3V3
Text Label 9100 1200 0    50   ~ 10
VS_3V3
Text Label 8800 2150 2    50   ~ 10
VS_3V3
Text Label 8850 3450 2    50   ~ 10
VS_3V3
Text Label 9350 2200 2    50   ~ 10
VS_3V3
Text Label 9350 3500 2    50   ~ 10
VS_3V3
Wire Wire Line
	9350 3500 9700 3500
Wire Wire Line
	9700 2200 9350 2200
$Comp
L power:+5V #PWR0102
U 1 1 639A094D
P 9850 6150
F 0 "#PWR0102" H 9850 6000 50  0001 C CNN
F 1 "+5V" H 9865 6323 50  0000 C CNN
F 2 "" H 9850 6150 50  0001 C CNN
F 3 "" H 9850 6150 50  0001 C CNN
	1    9850 6150
	-1   0    0    1   
$EndComp
$Comp
L power:+3.3V #PWR0105
U 1 1 639A3B0C
P 10300 6150
F 0 "#PWR0105" H 10300 6000 50  0001 C CNN
F 1 "+3.3V" H 10315 6323 50  0000 C CNN
F 2 "" H 10300 6150 50  0001 C CNN
F 3 "" H 10300 6150 50  0001 C CNN
	1    10300 6150
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 6399F38C
P 10750 6150
F 0 "#PWR0101" H 10750 5900 50  0001 C CNN
F 1 "GND" H 10750 6000 50  0000 C CNN
F 2 "" H 10750 6150 50  0001 C CNN
F 3 "" H 10750 6150 50  0001 C CNN
	1    10750 6150
	1    0    0    -1  
$EndComp
$Comp
L Device:C C7
U 1 1 63A107FA
P 8600 5400
F 0 "C7" H 8715 5446 50  0000 L CNN
F 1 "100nF" H 8715 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8638 5250 50  0001 C CNN
F 3 "~" H 8600 5400 50  0001 C CNN
	1    8600 5400
	1    0    0    -1  
$EndComp
Text Label 8600 5250 0    50   ~ 10
VS_5V
Wire Wire Line
	9100 5550 8600 5550
Connection ~ 9100 5550
Wire Notes Line
	750  7800 6050 7800
Text GLabel 5400 7650 2    50   Output ~ 0
RTC_ENABLE
$Comp
L Connector_Generic:Conn_02x01 J5
U 1 1 63A3D3DB
P 5100 7650
F 0 "J5" H 5150 7867 50  0000 C CNN
F 1 "RTC Enable" H 5150 7776 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 5100 7650 50  0001 C CNN
F 3 "~" H 5100 7650 50  0001 C CNN
	1    5100 7650
	1    0    0    -1  
$EndComp
Text Label 4900 7650 2    50   ~ 10
VS_3V3
Text Label 7650 2750 0    50   ~ 10
GND
Wire Wire Line
	7450 2750 7650 2750
Wire Wire Line
	7450 3250 7650 3250
Text Label 7650 3250 0    50   ~ 10
GND
Wire Wire Line
	7450 2250 7650 2250
Text Label 7650 2250 0    50   ~ 10
GND
Text Notes 6600 1450 0    50   ~ 0
Raspberry\nPI PICO
Text Label 6050 1750 2    50   ~ 10
GND
Text Label 6050 2250 2    50   ~ 10
GND
Text Label 5850 2750 2    50   ~ 10
GND
Text Label 5850 3250 2    50   ~ 10
GND
Wire Wire Line
	6050 2750 5850 2750
Wire Wire Line
	6050 3250 5850 3250
Text Label 2950 1550 0    50   ~ 0
AD0
Text Label 2950 1650 0    50   ~ 0
AD1
Text Label 2950 1750 0    50   ~ 0
AD2
Text Label 2950 1850 0    50   ~ 0
AD3
Text Label 2950 1950 0    50   ~ 0
AD4
Text Label 6000 1550 2    50   ~ 0
D0
Text Label 6000 1650 2    50   ~ 0
D1
Text Label 6000 1850 2    50   ~ 0
D2
Text Label 6000 1950 2    50   ~ 0
D3
Text Label 6000 2050 2    50   ~ 0
D4
Text Label 6000 2150 2    50   ~ 0
D5
Text Label 6000 2350 2    50   ~ 0
D6
Text Label 6000 2450 2    50   ~ 0
D7
Text Label 1050 1550 2    50   ~ 10
GND
Text Label 1050 1750 2    50   ~ 10
GND
Text Label 1050 1950 2    50   ~ 10
GND
Text Label 1050 2150 2    50   ~ 10
GND
Text Notes 5450 5550 0    50   ~ 10
PICO RESET
Text Notes 6250 4000 0    50   ~ 0
Between centres 17.78 mm\nWidth 21mm\nHeight 51 mm\n\n1 mm holes
$Comp
L Connector_Generic:Conn_01x01 J11
U 1 1 63836DA4
P 6250 1550
F 0 "J11" H 6300 1550 50  0000 L CNN
F 1 "GP0" H 6450 1550 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 6250 1550 50  0001 C CNN
F 3 "~" H 6250 1550 50  0001 C CNN
	1    6250 1550
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J12
U 1 1 63839540
P 6250 1650
F 0 "J12" H 6300 1650 50  0000 L CNN
F 1 "GP1" H 6450 1650 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 6250 1650 50  0001 C CNN
F 3 "~" H 6250 1650 50  0001 C CNN
	1    6250 1650
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J13
U 1 1 63839877
P 6250 1750
F 0 "J13" H 6300 1750 50  0000 L CNN
F 1 "GND" H 6450 1750 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 6250 1750 50  0001 C CNN
F 3 "~" H 6250 1750 50  0001 C CNN
	1    6250 1750
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J14
U 1 1 63839BCE
P 6250 1850
F 0 "J14" H 6300 1850 50  0000 L CNN
F 1 "GP2" H 6450 1850 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 6250 1850 50  0001 C CNN
F 3 "~" H 6250 1850 50  0001 C CNN
	1    6250 1850
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J15
U 1 1 63839EC6
P 6250 1950
F 0 "J15" H 6300 1950 50  0000 L CNN
F 1 "GP3" H 6450 1950 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 6250 1950 50  0001 C CNN
F 3 "~" H 6250 1950 50  0001 C CNN
	1    6250 1950
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J16
U 1 1 6383A160
P 6250 2050
F 0 "J16" H 6300 2050 50  0000 L CNN
F 1 "GP4" H 6450 2050 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 6250 2050 50  0001 C CNN
F 3 "~" H 6250 2050 50  0001 C CNN
	1    6250 2050
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J17
U 1 1 6383A348
P 6250 2150
F 0 "J17" H 6300 2150 50  0000 L CNN
F 1 "GP5" H 6450 2150 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 6250 2150 50  0001 C CNN
F 3 "~" H 6250 2150 50  0001 C CNN
	1    6250 2150
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J18
U 1 1 6383A681
P 6250 2250
F 0 "J18" H 6300 2250 50  0000 L CNN
F 1 "GND" H 6450 2250 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 6250 2250 50  0001 C CNN
F 3 "~" H 6250 2250 50  0001 C CNN
	1    6250 2250
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J19
U 1 1 6383A9EE
P 6250 2350
F 0 "J19" H 6300 2350 50  0000 L CNN
F 1 "GP6" H 6450 2350 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 6250 2350 50  0001 C CNN
F 3 "~" H 6250 2350 50  0001 C CNN
	1    6250 2350
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J20
U 1 1 63841B46
P 6250 2450
F 0 "J20" H 6300 2450 50  0000 L CNN
F 1 "GP7" H 6450 2450 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 6250 2450 50  0001 C CNN
F 3 "~" H 6250 2450 50  0001 C CNN
	1    6250 2450
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J21
U 1 1 63842186
P 6250 2550
F 0 "J21" H 6300 2550 50  0000 L CNN
F 1 "GP8" H 6450 2550 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 6250 2550 50  0001 C CNN
F 3 "~" H 6250 2550 50  0001 C CNN
	1    6250 2550
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J22
U 1 1 63842594
P 6250 2650
F 0 "J22" H 6300 2650 50  0000 L CNN
F 1 "GP9" H 6450 2650 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 6250 2650 50  0001 C CNN
F 3 "~" H 6250 2650 50  0001 C CNN
	1    6250 2650
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J23
U 1 1 638429E9
P 6250 2750
F 0 "J23" H 6300 2750 50  0000 L CNN
F 1 "GND" H 6450 2750 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 6250 2750 50  0001 C CNN
F 3 "~" H 6250 2750 50  0001 C CNN
	1    6250 2750
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J24
U 1 1 63842DEF
P 6250 2850
F 0 "J24" H 6300 2850 50  0000 L CNN
F 1 "GP10" H 6450 2850 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 6250 2850 50  0001 C CNN
F 3 "~" H 6250 2850 50  0001 C CNN
	1    6250 2850
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J25
U 1 1 63843131
P 6250 2950
F 0 "J25" H 6300 2950 50  0000 L CNN
F 1 "GP11" H 6450 2950 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 6250 2950 50  0001 C CNN
F 3 "~" H 6250 2950 50  0001 C CNN
	1    6250 2950
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J26
U 1 1 638435A1
P 6250 3050
F 0 "J26" H 6300 3050 50  0000 L CNN
F 1 "GP12" H 6450 3050 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 6250 3050 50  0001 C CNN
F 3 "~" H 6250 3050 50  0001 C CNN
	1    6250 3050
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J27
U 1 1 63844479
P 6250 3150
F 0 "J27" H 6300 3150 50  0000 L CNN
F 1 "GP13" H 6450 3150 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 6250 3150 50  0001 C CNN
F 3 "~" H 6250 3150 50  0001 C CNN
	1    6250 3150
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J28
U 1 1 6384482F
P 6250 3250
F 0 "J28" H 6300 3250 50  0000 L CNN
F 1 "GND" H 6450 3250 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 6250 3250 50  0001 C CNN
F 3 "~" H 6250 3250 50  0001 C CNN
	1    6250 3250
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J29
U 1 1 63844A92
P 6250 3350
F 0 "J29" H 6300 3350 50  0000 L CNN
F 1 "GP14" H 6450 3350 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 6250 3350 50  0001 C CNN
F 3 "~" H 6250 3350 50  0001 C CNN
	1    6250 3350
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J30
U 1 1 63844CC7
P 6250 3450
F 0 "J30" H 6300 3450 50  0000 L CNN
F 1 "GP15" H 6450 3450 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 6250 3450 50  0001 C CNN
F 3 "~" H 6250 3450 50  0001 C CNN
	1    6250 3450
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J31
U 1 1 638459F4
P 7250 3450
F 0 "J31" H 7300 3450 50  0000 L CNN
F 1 "GP16" H 7450 3450 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 7250 3450 50  0001 C CNN
F 3 "~" H 7250 3450 50  0001 C CNN
	1    7250 3450
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J32
U 1 1 63846E71
P 7250 3350
F 0 "J32" H 7300 3350 50  0000 L CNN
F 1 "GP17" H 7450 3350 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 7250 3350 50  0001 C CNN
F 3 "~" H 7250 3350 50  0001 C CNN
	1    7250 3350
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J33
U 1 1 63847314
P 7250 3250
F 0 "J33" H 7300 3250 50  0000 L CNN
F 1 "GND" H 7450 3250 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 7250 3250 50  0001 C CNN
F 3 "~" H 7250 3250 50  0001 C CNN
	1    7250 3250
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J34
U 1 1 638475BF
P 7250 3150
F 0 "J34" H 7300 3150 50  0000 L CNN
F 1 "GP18" H 7450 3150 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 7250 3150 50  0001 C CNN
F 3 "~" H 7250 3150 50  0001 C CNN
	1    7250 3150
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J35
U 1 1 638478B5
P 7250 3050
F 0 "J35" H 7300 3050 50  0000 L CNN
F 1 "GP19" H 7450 3050 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 7250 3050 50  0001 C CNN
F 3 "~" H 7250 3050 50  0001 C CNN
	1    7250 3050
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J36
U 1 1 63847E24
P 7250 2950
F 0 "J36" H 7300 2950 50  0000 L CNN
F 1 "GP20" H 7450 2950 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 7250 2950 50  0001 C CNN
F 3 "~" H 7250 2950 50  0001 C CNN
	1    7250 2950
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J37
U 1 1 638481B5
P 7250 2850
F 0 "J37" H 7300 2850 50  0000 L CNN
F 1 "GP21" H 7450 2850 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 7250 2850 50  0001 C CNN
F 3 "~" H 7250 2850 50  0001 C CNN
	1    7250 2850
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J38
U 1 1 6384858E
P 7250 2750
F 0 "J38" H 7300 2750 50  0000 L CNN
F 1 "GND" H 7450 2750 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 7250 2750 50  0001 C CNN
F 3 "~" H 7250 2750 50  0001 C CNN
	1    7250 2750
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J39
U 1 1 638489A2
P 7250 2650
F 0 "J39" H 7300 2650 50  0000 L CNN
F 1 "GP22" H 7450 2650 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 7250 2650 50  0001 C CNN
F 3 "~" H 7250 2650 50  0001 C CNN
	1    7250 2650
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J40
U 1 1 63848D24
P 7250 2550
F 0 "J40" H 7300 2550 50  0000 L CNN
F 1 "RUN" H 7450 2550 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 7250 2550 50  0001 C CNN
F 3 "~" H 7250 2550 50  0001 C CNN
	1    7250 2550
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J41
U 1 1 638495BD
P 7250 2450
F 0 "J41" H 7300 2450 50  0000 L CNN
F 1 "GP26" H 7450 2450 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 7250 2450 50  0001 C CNN
F 3 "~" H 7250 2450 50  0001 C CNN
	1    7250 2450
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J42
U 1 1 63849859
P 7250 2350
F 0 "J42" H 7300 2350 50  0000 L CNN
F 1 "GP27" H 7450 2350 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 7250 2350 50  0001 C CNN
F 3 "~" H 7250 2350 50  0001 C CNN
	1    7250 2350
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J43
U 1 1 63849C42
P 7250 2250
F 0 "J43" H 7300 2250 50  0000 L CNN
F 1 "GND" H 7450 2250 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 7250 2250 50  0001 C CNN
F 3 "~" H 7250 2250 50  0001 C CNN
	1    7250 2250
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J44
U 1 1 6384A030
P 7250 2150
F 0 "J44" H 7300 2150 50  0000 L CNN
F 1 "GP28" H 7450 2150 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 7250 2150 50  0001 C CNN
F 3 "~" H 7250 2150 50  0001 C CNN
	1    7250 2150
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J45
U 1 1 6384A431
P 7250 2050
F 0 "J45" H 7300 2050 50  0000 L CNN
F 1 "VREF" H 7450 2050 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 7250 2050 50  0001 C CNN
F 3 "~" H 7250 2050 50  0001 C CNN
	1    7250 2050
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J46
U 1 1 6384A707
P 7250 1950
F 0 "J46" H 7300 1950 50  0000 L CNN
F 1 "VSS_3V3" H 7450 1950 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 7250 1950 50  0001 C CNN
F 3 "~" H 7250 1950 50  0001 C CNN
	1    7250 1950
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J47
U 1 1 6384AAAE
P 7250 1850
F 0 "J47" H 7300 1850 50  0000 L CNN
F 1 "3V3_EN" H 7450 1850 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 7250 1850 50  0001 C CNN
F 3 "~" H 7250 1850 50  0001 C CNN
	1    7250 1850
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J48
U 1 1 6384AEDD
P 7250 1750
F 0 "J48" H 7300 1750 50  0000 L CNN
F 1 "GND" H 7450 1750 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 7250 1750 50  0001 C CNN
F 3 "~" H 7250 1750 50  0001 C CNN
	1    7250 1750
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J49
U 1 1 6384B2F7
P 7250 1650
F 0 "J49" H 7300 1650 50  0000 L CNN
F 1 "VSYS" H 7450 1650 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 7250 1650 50  0001 C CNN
F 3 "~" H 7250 1650 50  0001 C CNN
	1    7250 1650
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J50
U 1 1 6384B74B
P 7250 1550
F 0 "J50" H 7300 1550 50  0000 L CNN
F 1 "VSS_5V" H 7450 1550 50  0000 L CNN
F 2 "TestPoint:SMD_Pad_3.0x1.5mm" H 7250 1550 50  0001 C CNN
F 3 "~" H 7250 1550 50  0001 C CNN
	1    7250 1550
	-1   0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 6387DF32
P 10300 6050
F 0 "#FLG0101" H 10300 6125 50  0001 C CNN
F 1 "PWR_FLAG" H 10300 6223 50  0000 C CNN
F 2 "" H 10300 6050 50  0001 C CNN
F 3 "~" H 10300 6050 50  0001 C CNN
	1    10300 6050
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 6387E3D1
P 9850 6050
F 0 "#FLG0102" H 9850 6125 50  0001 C CNN
F 1 "PWR_FLAG" H 9850 6223 50  0000 C CNN
F 2 "" H 9850 6050 50  0001 C CNN
F 3 "~" H 9850 6050 50  0001 C CNN
	1    9850 6050
	1    0    0    -1  
$EndComp
Text Label 3500 5800 0    50   ~ 10
VS_5V
Text Label 1450 6700 2    50   ~ 10
VS_5V
Text GLabel 3950 3750 2    50   Input ~ 0
CNTRLBUSENABLE
Text GLabel 7450 2150 2    50   Output ~ 0
CNTRLBUSENABLE
$Comp
L Device:R_Small R6
U 1 1 63835509
P 6500 1000
F 0 "R6" H 6300 1050 50  0000 L CNN
F 1 "R1K" H 6300 950 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 6500 1000 50  0001 C CNN
F 3 "~" H 6500 1000 50  0001 C CNN
	1    6500 1000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 63836804
P 6500 1100
F 0 "#PWR0103" H 6500 850 50  0001 C CNN
F 1 "GND" H 6505 927 50  0000 C CNN
F 2 "" H 6500 1100 50  0001 C CNN
F 3 "" H 6500 1100 50  0001 C CNN
	1    6500 1100
	1    0    0    -1  
$EndComp
Text GLabel 6500 850  2    50   Input ~ 0
CNTRLBUSENABLE
Text Notes 6500 750  0    50   ~ 0
Make sure CNTRLBUSENABLE is\nlow on power-up
Wire Wire Line
	6500 900  6500 850 
Text Label 9750 5250 0    50   ~ 10
VS_5V
Text Label 3150 4450 2    50   ~ 10
GND
Wire Wire Line
	3950 4350 3950 4450
Wire Wire Line
	3150 4350 3150 4450
Text Notes 3750 4950 0    50   ~ 0
Unused pins must be grounded
Text Label 3950 4450 0    50   ~ 10
GND
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J4
U 1 1 6397A2EF
P 1650 6800
F 0 "J4" H 1700 6475 50  0000 C CNN
F 1 "ID Select" H 1700 6566 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x03_P2.54mm_Vertical" H 1650 6800 50  0001 C CNN
F 3 "~" H 1650 6800 50  0001 C CNN
	1    1650 6800
	1    0    0    1   
$EndComp
Wire Wire Line
	1450 6700 1450 6800
Connection ~ 1950 6900
Wire Wire Line
	1450 6900 1450 6800
Connection ~ 1450 6800
$Comp
L Device:R_Small R18
U 1 1 63982769
P 2550 7150
F 0 "R18" H 2350 7100 50  0000 L CNN
F 1 "R10K" H 2300 7000 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 2550 7150 50  0001 C CNN
F 3 "~" H 2550 7150 50  0001 C CNN
	1    2550 7150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 7550 2550 7550
Connection ~ 2250 7550
Wire Wire Line
	1950 7050 1950 6900
Wire Wire Line
	1950 7250 1950 7550
Wire Wire Line
	1950 6700 2550 6700
Wire Wire Line
	2550 7050 2550 6700
Connection ~ 2550 6700
Wire Wire Line
	2550 6700 3000 6700
Wire Wire Line
	2550 7250 2550 7550
$EndSCHEMATC
