#ifndef IHWLAYER_H
#define IHWLAYER_H

#define BUTTON_DRUM_1 16
#define BUTTON_DRUM_2 17
#define BUTTON_DRUM_3 18

class IHWLayer {
public:

	enum LedState{ON, OFF, BLINK, BLINK_INVERT, DULLON};
	enum ButtonState{UP, DOWN};


    virtual ButtonState getButtonState(unsigned char index) = 0;
    virtual void setLED(unsigned char index, LedState state) = 0;
    virtual unsigned char getKnobValue(unsigned char index) = 0;
    virtual unsigned char readSRAM(long address) = 0;
    virtual void readSRAM(long address, unsigned char * data, unsigned int size) = 0;
    virtual void writeSRAM(long address, unsigned char data) = 0;
    virtual void writeSRAM(long address, unsigned char * data, unsigned int size) = 0;
    virtual unsigned int getElapsedBastlCycles() = 0;
    virtual unsigned int getBastlCyclesPerSecond() = 0;

};

#endif // IHWLAYER_H
