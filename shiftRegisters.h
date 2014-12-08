/*
 * shiftRegisters.h
 *
 *  Created on: 06.07.2014
 *      Author: user
 */

#include <portManipulations.h>
#include <inttypes.h>

#ifndef SHIFTREGISTERS_H_
#define SHIFTREGISTERS_H_



// Class to write data to 595 type shift registers
//
// after creating an instance, use setup() to set the pins you are using. three pins are need for operation:
//   - SER:  data input to the shift register
//   - SRCK: clock for taking the value from SER to the first storage register and shifting the other one step ahead
//   - RCK:  enable the output of the shift register
//
// - if you have several shift registers daisy chained, you can use write() multiple times
// - shifting out a byte takes ~70us due to continuous pointer dereferencing but you can access multiple shift registers with multiple instances

class shiftRegisterBasic {
public:

	// empty. do not use!
	shiftRegisterBasic();

	// set up the pins you want to use
	void setup(
		 volatile uint8_t* srck_port,
		 volatile uint8_t* srck_ddr,
		          uint8_t  srck_num,
		 volatile uint8_t* rck_port,
		 volatile uint8_t* rck_ddr,
		 	 	  uint8_t  rck_num,
		 volatile uint8_t* ser_port,
		 volatile uint8_t* ser_ddr,
		 	 	  uint8_t  ser_num);

	// write a byte to the shift register
	// you have to enable the output manually
	void write(uint8_t byte);

	// enable output
	inline void enableOutput() {
		setHigh(*rck_port,rck_num);
		setLow(*rck_port,rck_num);
	}

	// clock in a single bit
	inline void serClkIn() {
		setHigh(*srck_port,srck_num);
		setLow(*srck_port,srck_num);
	}

	// convenient method for setting the value of SER pin
	inline void setDataBit(bool value) {
		if (value) setHigh(*ser_port,ser_num);
		else 	   setLow(*ser_port,ser_num);
	}


private:
	volatile uint8_t* srck_port;  // the port SRCK pin is on
	uint8_t			  srck_num;   // the number of the SRCK pin inside the SRCK port
	volatile uint8_t* rck_port;   // ..same here
	uint8_t			  rck_num;    // ...
	volatile uint8_t* ser_port;
	uint8_t           ser_num;
};


#endif /* SHIFTREGISTERS_H_ */

