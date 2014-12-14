#ifndef ILEDHW_H
#define ILEDHW_H

#define BUTTON_DRUM_1 16
#define BUTTON_DRUM_2 17
#define BUTTON_DRUM_3 18

#include <inttypes.h>

class ILEDHW {
public:

	enum LedState{ON,OFF, BLINK, BLINK_INVERT, DULLON};
	
    // LEDs //
    virtual void setLED(uint8_t index, LedState state) = 0;

};

#endif // ILEDHW_H
