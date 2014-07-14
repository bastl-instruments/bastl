
#ifndef SEKVOJHW_H_
#define SEKVOJHW_H_



#define SHIFTREGISTER_SER C,5
#define SHIFTREGISTER_RCK B,1
#define SHIFTREGISTER_SRCK B,0


#define BUTTONCOL_0 C,1
#define BUTTONCOL_1 C,0
#define BUTTONCOL_2 D,7
#define BUTTONCOL_3 D,6

#define SS_SDCARD B,2
#define SS_RAM    D,2


static const uint8_t updateFreq = 50; // in Hertz


// for RAM
#define NOP asm volatile ("nop\n\t")
#define UINT16_MAX 65535
#define SCK B,5
#define MISO B,4
#define MOSI B,3


class sekvojHW {


public:

	enum buttonState{pressed,released};
	enum ledState{on,off,blinking};

	//sekvojHW(){};
	void setup();

	void leds_printStates();
	void leds_setStates(uint16_t ledStates[]);
	void leds_allOff();
	void leds_update();
	void leds_updateNextRow();
	void led_setOn(uint8_t number);
	void led_setOff(uint8_t number);

	void buttons_update();
	void buttons_printStates();
	buttonState button_getState(uint8_t number);

	void display_start();
	void display_clear();

	void writeSRAM(long address, uint8_t data);
	void writeSRAM(long address, uint8_t* buf, uint16_t len);
	uint8_t readSRAM(long address);
	void readSRAM(long address, uint8_t* buf, uint16_t len);

private:

	void display_sendCommand(uint8_t command);
	void display_sendData(uint8_t data);
	void display_send(uint8_t byte);
	void display_enable();

	uint16_t ledStates[4];
	uint16_t buttonStates[4];

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
