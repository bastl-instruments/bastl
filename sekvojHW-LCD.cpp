/*
 * SEKVOJ_HW.cpp
 *
 *  Created on: 11.07.2014
 *      Author: user
 */

#include <Arduino.h>
#include <sekvojHW.h>



#include <sekvojHW-settings.h>
#include <shiftRegisterFast.h>


// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// additional pins
#define RS_Pin C,2
#define ENABLE_Pin C,3

#define PIN D,3

void sekvojHW::display_start() {

	bit_set(PIN);

	bit_dir_outp(RS_Pin);
	bit_dir_outp(ENABLE_Pin);



	bit_clear(RS_Pin);        //set pins low to start commands
	bit_clear(ENABLE_Pin);

	_displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
	_displayfunction |= LCD_2LINE;


	delayMicroseconds(50000); // wait for display to power on


	// set display mode
    display_sendCommand(LCD_FUNCTIONSET | LCD_8BITMODE | LCD_2LINE);
    delayMicroseconds(4500);  // wait more than 4.1ms
    display_sendCommand(LCD_FUNCTIONSET | LCD_8BITMODE | LCD_2LINE);
    delayMicroseconds(150);
    display_sendCommand(LCD_FUNCTIONSET | LCD_8BITMODE | LCD_2LINE);
    display_sendCommand(LCD_FUNCTIONSET | LCD_8BITMODE | LCD_2LINE);

    display_sendCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);

    display_clear();

    display_sendCommand(LCD_ENTRYMODESET |  LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);

    display_sendData(65);

    bit_clear(PIN);



}

void sekvojHW::display_sendCommand(uint8_t command) {
	bit_clear(RS_Pin);
	display_send(command);
}

void sekvojHW::display_sendData(uint8_t data) {
	bit_set(RS_Pin);
	display_send(data);
}

void sekvojHW::display_send(uint8_t byte) {
	shiftRegFast::write_8bit(byte,shiftRegFast::msbfirst);
	shiftRegFast::write_8bit(0);
	display_enable();
}

void sekvojHW::display_enable() {
	  bit_clear(ENABLE_Pin);
	  delayMicroseconds(1);
	  bit_set(ENABLE_Pin);
	  delayMicroseconds(1);    // enable pulse must be >450ns
	  bit_clear(ENABLE_Pin);
	  delayMicroseconds(100);   // commands need > 37us to settle
}

void sekvojHW::display_clear() {
	  display_sendCommand(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
	  delayMicroseconds(2000);  			  // this command takes a long time!
}

