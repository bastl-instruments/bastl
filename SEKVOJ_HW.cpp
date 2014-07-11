/*
 * SEKVOJ_HW.cpp
 *
 *  Created on: 11.07.2014
 *      Author: user
 */

#include <Arduino.h>
#include <SEKVOJ_HW.h>
#include <shiftRegisterFast.h>


#define timerCompareValue

static const uint8_t leds_cols = 12;
static const uint8_t leds_rows = 4;
static const uint8_t buttons_cols = 4;
static const uint8_t buttons_rows = 12;

static const uint8_t rowsTotal = 5;



// Declaration of instance
// used in interrupt service routine
sekvojHW hardware;





void sekvojHW::setup() {

	cli();

	shiftRegFast::setup();

	// BUTTONS
	bit_dir_inp(BUTTONCOL_0);
	bit_dir_inp(BUTTONCOL_1);
	bit_dir_inp(BUTTONCOL_2);
	bit_dir_inp(BUTTONCOL_3);

	bit_set(BUTTONCOL_0);
	bit_set(BUTTONCOL_1);
	bit_set(BUTTONCOL_2);
	bit_set(BUTTONCOL_3);

	// LEDS
	leds_allOff();

	// TIMER
	TCCR2A = (1 << WGM21);  // turn on CTC mode
	TIMSK2 |= (1 << OCIE2A);// enable interrupt
	TCCR2B = B00000111;	  //prescaler = 1024
	OCR2A = (F_CPU/1024)/(updateFreq*rowsTotal);
	TCNT2  = 0;

	// DISPLAY
	display_start();

	//sei();
}


void sekvojHW::leds_printStates() {
	for (uint8_t row=0; row<leds_rows; row++) {
		Serial.print("Row "); Serial.print(row,DEC);Serial.print(": ");
		for (int8_t col=15; col>=0;col--) {
			if (bitRead(ledStates[row],col)) {
				Serial.print("1");
			} else {
				Serial.print("0");
			}
		}
		Serial.println("");
	}
}



void sekvojHW::leds_setStates(uint16_t ledStates[]) {
	for (uint8_t row = 0; row<leds_rows; row++) {
		this->ledStates[row]=ledStates[row];

	}
}


void sekvojHW::led_setOn(uint8_t number) {
	ledStates[number/leds_cols] &= ~(1<<(number%leds_cols));

}

void sekvojHW::led_setOff(uint8_t number) {
	ledStates[number/leds_cols] |= (1<<(number%leds_cols));

}

void sekvojHW::leds_allOff() {
	for (uint8_t row=0; row<leds_rows; row++) {
		ledStates[row] =  1<<(15-row);    				//set row hit high
		ledStates[row] |= (B00001111<<8) | (B11111111); //disable all rows
	}
}

void sekvojHW::leds_update() {
	   for (uint8_t row =0; row<leds_rows; row++) {
		   shiftRegFast::write(ledStates[row]);
		   shiftRegFast::enableOutput();
	   }
}

void sekvojHW::leds_updateNextRow() {
	static uint8_t currentRow = 0;
	shiftRegFast::write(ledStates[currentRow]);
	shiftRegFast::enableOutput();
	currentRow=(currentRow+1)%leds_rows;
}


void sekvojHW::buttons_update() {

	for (int8_t row=buttons_rows-1; row>=0; row--) {

		shiftRegFast::write((uint16_t)(0xFFF & ~(1<<row)));
		shiftRegFast::enableOutput();

		bitWrite(buttonStates[0],row, bit_read_in(BUTTONCOL_0));
		bitWrite(buttonStates[1],row, bit_read_in(BUTTONCOL_1));
		bitWrite(buttonStates[2],row, bit_read_in(BUTTONCOL_2));
		bitWrite(buttonStates[3],row, bit_read_in(BUTTONCOL_3));
	}
}


void sekvojHW::buttons_printStates() {
	for (uint8_t row=0; row<4; row++) {
		Serial.print("col "); Serial.print(row,DEC);Serial.print(": ");
		for (int8_t col=15; col>=0;col--) {
			if (bitRead(buttonStates[row],col)) {
				Serial.print("1");
			} else {
				Serial.print("0");
			}
		}
		Serial.println("");
	}
}

sekvojHW::buttonState sekvojHW::button_getState(uint8_t number) {

	if ((buttonStates[number/buttons_rows] & (1<<(number%buttons_rows)))) {
		return released;
	} else {
		return pressed;
	}


}


ISR(TIMER2_COMPA_vect) {

		static uint8_t counter= 0;

		if (counter < 3)  {
			hardware.buttons_update();
			counter++;
		} else {
			counter=0;
		}

		hardware.leds_updateNextRow();





}


