#ifndef IHWLAYER_H
#define IHWLAYER_H

#define BUTTON_DRUM_1 16
#define BUTTON_DRUM_2 17
#define BUTTON_DRUM_3 18

#include <inttypes.h>
#include "ILEDHW.h"

class IHWLayer : public ILEDHW {
public:

	enum ButtonState{UP, DOWN};
	enum EventType {EMPTY, BTN_DOWN, BTN_UP, KNOB_TWIST};

	// Buttons //
    virtual ButtonState getButtonState(unsigned char index) = 0;

    // Knobs //
    virtual unsigned char getKnobValue(unsigned char index) = 0;
    virtual void freezeKnob(uint8_t number, uint8_t value) = 0;
    virtual void setKnobLED(uint8_t number, uint8_t value) = 0;
    virtual void setKnobLEDTracking(uint8_t number, bool) = 0;


    // RAM //
    virtual unsigned char readSRAM(long address) = 0;
    virtual void readSRAM(long address, unsigned char * data, unsigned int size) = 0;
    virtual void writeSRAM(long address, unsigned char data) = 0;
    virtual void writeSRAM(long address, unsigned char * data, unsigned int size) = 0;

    // Timing
    virtual unsigned int getElapsedBastlCycles() = 0;
    virtual unsigned int getBastlCyclesPerSecond() = 0;

    // EEPROM
    virtual bool writeEEPROM(uint16_t address, uint8_t* byteArray, uint16_t numbBytes);
    virtual void readEEPROM(uint16_t address, uint8_t* byteArray, uint16_t numbBytes);

    // general operation
    virtual void giveSomeTime();
    virtual void giveAllTime();

};

struct hardwareEvent{
	uint8_t number;
	IHWLayer::EventType type;
	uint16_t time;
};

const hardwareEvent emptyEvent = {255,IHWLayer::EMPTY,0};


#endif // IHWLAYER_H
