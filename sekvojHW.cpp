
/*** PERMANENT SETTINGS***/

#include <sekvojHW-settings.h>



/*** ACTUAL CODE ***/

#include <Arduino.h>
#include <sekvojHW.h>
#include <shiftRegisterFast.h>
//#include <SdFat.h>

// Declaration of instance (for use in interrupt service routine)
sekvojHW hardware;


#define UINT16_MAX 65535
#define MAX_ADDR 131067

// set by defines
static const uint8_t updateFreq = FREQ;
static const uint8_t blinkCompare[2] = {blinkDuty,blinkTotal};

// set by hardware
static const uint8_t leds_cols = 12;
static const uint8_t leds_rows = 4;
static const uint8_t buttons_cols = 4;
static const uint8_t buttons_rows = 12;

static const uint8_t rowsTotal = 4; // for calculation of update frequency timer



// internal defines for RAM
#define NOP asm volatile ("nop\n\t")

#define SCK B,5
#define MISO B,4
#define MOSI B,3





void sekvojHW::init() {

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
	leds_init();

	// SPI
	bit_dir_outp(SCK);
	bit_dir_outp(MOSI);
	bit_dir_inp(MISO);
	bit_dir_outp(SS_RAM);
	bit_dir_outp(SS_SDCARD);

	bit_clear(SCK);
	bit_clear(MOSI);
	bit_clear(MISO);
	bit_set(SS_RAM);
	bit_set(SS_SDCARD);

	// Configure SPI
	SPCR |= _BV(SPE);    // enable SPI
	SPCR &= ~_BV(SPIE);  // SPI interrupts off
	SPCR &= ~_BV(DORD);  // MSB first
	SPCR |= _BV(MSTR);   // SPI master mode
	SPCR &= ~_BV(CPOL);  // leading edge rising
	SPCR &= ~_BV(CPHA);  // sample on leading edge
	SPCR &= ~_BV(SPR1);  // speed = clock/4
	SPCR &= ~_BV(SPR0);
	SPSR |= _BV(SPI2X);  // 2X speed

	// Disable Timer1 interrupt
	//TIMSK1 &= ~_BV(TOIE1);

	// TIMER 2
	TCCR2A = (1 << WGM21);  // turn on CTC mode
	TIMSK2 |= (1 << OCIE2A);// enable interrupt
	TCCR2B = B00000111;	  //prescaler = 1024
	OCR2A = (F_CPU/1024)/(updateFreq*rowsTotal);
	TCNT2  = 0;

	// DISPLAY
	display_start();
	sei();
}


/**** LEDS ****/

void sekvojHW::leds_printStates() {
	for (uint8_t row=0; row<leds_rows; row++) {
		Serial.print("Row "); Serial.print(row,DEC);Serial.print(": ");
		for (int8_t col=15; col>=0;col--) {
			if (bitRead(ledStatesBeg[row],col)) {
				Serial.print("1");
			} else {
				Serial.print("0");
			}
		}
		Serial.println("");
		Serial.print("    "); Serial.print(row,DEC);Serial.print(": ");
		for (int8_t col=15; col>=0;col--) {
			if (bitRead(ledStatesEnd[row],col)) {
				Serial.print("1");
			} else {
				Serial.print("0");
			}
		}
		Serial.println("");
	}
}



/*void sekvojHW::leds_setStates(uint16_t ledStates[]) {
	for (uint8_t row = 0; row<leds_rows; row++) {
		this->ledStatesBeg[row]=ledStates[row];

	}
}*/


void sekvojHW::led_setState(uint8_t number,sekvojHW::ledState state) {

	if ((state == on) | (state==blinkStart)) {
		ledStatesBeg[number/leds_cols] &= ~(1<<(number%leds_cols));
	} else {
		ledStatesBeg[number/leds_cols] |= (1<<(number%leds_cols));

	}

	if ((state == on) | (state==blinkEnd)) {
		ledStatesEnd[number/leds_cols] &= ~(1<<(number%leds_cols));
	} else {
		ledStatesEnd[number/leds_cols] |= (1<<(number%leds_cols));

	}
}



void sekvojHW::leds_init() {
	for (uint8_t row=0; row<leds_rows; row++) {
		ledStatesBeg[row] =  1<<(15-row);    				//set row hit high
		ledStatesBeg[row] |= (B00001111<<8) | (B11111111); //disable all rows

		ledStatesEnd [row] = ledStatesBeg[row]; 			// copy to second set of states
	}

}



void sekvojHW::leds_updateNextRow() {
	static uint8_t currentRow = 0;
	static uint8_t blinkCounter = 0;

	if (blinkCounter < blinkCompare[0]) {
		shiftRegFast::write_16bit(ledStatesBeg[currentRow]);
	} else {
		shiftRegFast::write_16bit(ledStatesEnd[currentRow]);
	}

	shiftRegFast::enableOutput();

	currentRow=(currentRow+1)%leds_rows;
	if (currentRow == 0) blinkCounter = (blinkCounter+1)%blinkCompare[1];
}



/**** BUTTONS ****/


void sekvojHW::buttons_update() {

	for (int8_t row=buttons_rows-1; row>=0; row--) {

		shiftRegFast::write_16bit((0xFFF & ~(1<<row)));
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



/**** RAM ****/

void sekvojHW::writeSRAM(long address, uint8_t data) {

	bit_clear(SS_RAM);

	spiWrite(0x02);           // mode = write
	spiWrite(address >> 16);  // address
	spiWrite(address >> 8);
	spiWrite(address);
	spiWrite(data);

	bit_set(SS_RAM);
}

void sekvojHW::writeSRAM(long address, uint8_t* buf, uint16_t len) {

	bit_clear(SS_RAM);

  spiWrite(0x02);           // mode = write
  spiWrite(address >> 16);  // address
  spiWrite(address >> 8);
  spiWrite(address);
  for(uint16_t i=0;i<len;i++) {
    spiWrite(*buf);
    buf++;
  }

  bit_set(SS_RAM);
}

uint8_t sekvojHW::readSRAM(long address) {
  uint8_t data;

  bit_clear(SS_RAM);

  spiWrite(0x03);           // mode = read
  spiWrite(address >> 16);  // address
  spiWrite(address >> 8);
  spiWrite(address);
  data = spiRead();

  bit_set(SS_RAM);

  return data;
}

void sekvojHW::readSRAM(long address, uint8_t* buf, uint16_t len) {

	bit_clear(SS_RAM);

  spiWrite(0x03);           // mode = read
  spiWrite(address >> 16);  // address
  spiWrite(address >> 8);
  spiWrite(address);
  for(uint16_t i=0;i<len;i++) {
    *buf = spiRead();
    buf++;
  }

  bit_set(SS_RAM);

}


/**** INTERRUPT ****/

ISR(TIMER2_COMPA_vect) {

	hardware.buttons_update();
	hardware.leds_updateNextRow();

}


