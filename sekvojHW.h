
#ifndef SEKVOJHW_H_
#define SEKVOJHW_H_





class sekvojHW {



public:

	enum buttonState{pressed,released};
	enum ledState{on,off,blinkStart,blinkEnd};

	// sets up all the pins, timers and SPI interface
	// call this before using any other method from this class
	void setup();


	/***LEDS***/

	// turn off all leds and reformat the state arrays
	// this is automatically called duribng setup
	void leds_init();

	// set the state of a led
	void led_setState(uint8_t number,ledState state);

	// print the state arrays to the Serial terminal
	void leds_printStates();


	/***BUTTONS***/

	// print the read button states to serial terminal
	void buttons_printStates();

	// the the state of a button identified by its id
	buttonState button_getState(uint8_t number);


	/***RAM***/

	// write a byte to the given address
	void writeSRAM(long address, uint8_t data);

	// write a number of bytes starting at the given address
	void writeSRAM(long address, uint8_t* buf, uint16_t len);

	// read the byte stored at the given address
	uint8_t readSRAM(long address);

	// read a number of bytes starting from the given address
	void readSRAM(long address, uint8_t* buf, uint16_t len);



	/***DISPLAY***/
	void display_start();
	void display_clear();

	// only called by ISR routine.
	// the would be declared private but this would prevent the ISR to access them
	// there are workarounds for this but as they come at a cost I just left it like this
	void leds_updateNextRow();
	void buttons_update();

private:


	void display_sendCommand(uint8_t command);
	void display_sendData(uint8_t data);
	void display_send(uint8_t byte);
	void display_enable();

	uint16_t ledStatesBeg[4];
	uint16_t ledStatesEnd[4];
	uint16_t buttonStates[4];

	uint8_t _displayfunction;
	uint8_t _displaycontrol;
	uint8_t _displaymode;

};

static inline __attribute__((always_inline)) byte spiRead() {
  SPDR = 0xFF; // start SPI clock
  while (!(SPSR & _BV(SPIF)));
  return SPDR;
}

static inline __attribute__((always_inline)) byte spiWrite(byte data) {

  SPDR = data;
  while (!(SPSR & _BV(SPIF)));

  return SPDR;
}





#endif
