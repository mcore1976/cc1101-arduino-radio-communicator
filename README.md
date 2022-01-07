# cc1101-arduino-radio-communicator
This is simple half duplex radio communication device built with CC1101 module and Arduino Pro Micro (3.3V / 8MHz) using SmartRC library
You simply connect your Arduino Pro Micro (Arduino Leonardo clone from Sparkfun) to USB port and launch Putty terminal to communicate over USB Serial port ( /dev/ttyACM0 port in Linux,  COMxx in Windows)

The code code uses SmartRC library (modified Electrohouse library by Little_S@tan) which allows to customize ALL transmission parameters in human readable format without using SmartRF studio from TI (CC1101 parameter customization tool). To use it please download following ZIP library from following github link https://github.com/LSatan/SmartRC-CC1101-Driver-Lib  and attach it to the script in Arduino IDE.


Arduino Pro Micro board ( ATMEGA32U4 chip ) must support 3.3Volt VCC and 3.3V TTL logic because this is required by CC1101 board, otherwise you will fry CC1101 chip. Please follow this guide to setup your Arduino environment for Arduino Pro Micro board : https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/all

If you are having issues with uploading the code from Arduino IDE to the board, after pressing "Upload" in Arduino you have to immediatelly short GND+RST pins two times in few seconds. Then bootloader in Arduino Pro Micro will start (common issue) and upload will begin.

Connections to be made :

ARDUINO PRO MICRO 3.3V / 8MHz <-> CC1101 BOARD

    DIGITAL PIN 3 ( PD0 / INT0 ) <-> CC1101 GDO0
    DIGITAL PIN 9 ( PB5 ) <-> CC1101 GDO2
    DIGITAL PIN 10 ( PB6 ) <-> CC1101 CSN / CS / SS
    DIGITAL PIN 16 ( PB2 / MOSI ) <-> CC1101 MOSI
    DIGITAL PIN 14 ( PB3 / MISO ) <-> CC1101 MISO
    DIGITAL PIN 15 ( PB1 / SCK ) <-> CC1101 SCLK / CLK
