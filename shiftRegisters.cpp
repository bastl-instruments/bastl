/*
 * shiftRegisters.cpp
 *
 *  Created on: 06.07.2014
 *      Author: user
 */

#include <shiftRegisters.h>
#include <portManipulations.h>
#include <Arduino.h>



shiftRegisterBasic::shiftRegisterBasic() {
	//should not be called
}

void shiftRegisterBasic::setup(
		 volatile uint8_t* srck_port,
		 volatile uint8_t* srck_ddr,
		          uint8_t  srck_num,
		 volatile uint8_t* rck_port,
		 volatile uint8_t* rck_ddr,
		 	 	  uint8_t  rck_num,
		 volatile uint8_t* ser_port,
		 volatile uint8_t* ser_ddr,
		 	 	  uint8_t  ser_num) {

	//save pointers to ports
	this->srck_port = srck_port;
	this->srck_num = srck_num;
	this->rck_port = rck_port;
	this->rck_num = rck_num;
	this->ser_port = ser_port;
	this->ser_num = ser_num;


	//set pins as outputs
	setHigh(*srck_ddr,srck_num);
	setHigh(*rck_ddr,rck_num);
	setHigh(*ser_ddr,ser_num);

	//initialize clocks to low
	setLow(*rck_port,rck_num);
	setLow(*srck_port,srck_num);

}

void shiftRegisterBasic::write(uint8_t byte) {

	cli();

	uint8_t dataHigh = *ser_port | (1<<ser_num);
	uint8_t dataLow = dataHigh & ~(1<<ser_num);

	for (uint8_t index=0; index<8; index++) {

		if ((byte >> index) & 1) *ser_port = dataHigh;
		else			         *ser_port = dataLow;

		//clock it in. only set bits to not influence SER port
		uint8_t clkHigh = *srck_port | (1<<srck_num);
		*srck_port = clkHigh;
		*srck_port = clkHigh & ~(1<<srck_num);

	}

	sei();
}


