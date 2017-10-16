#include "Switches.h"
#include "BitArrayOperations.h"

//#define DEBUG

#ifdef DEBUG
#include <cstdio>
#endif

Switches::Switches() :
    buttonIndexes_(0),
    buttonCount_(0),
    lastStates_(0),
    statuses_(0),
    changeOnEvent_(IButtonHW::DOWN),
    useLEDs_(false)
{
}

void Switches::init(unsigned char * buttonIndexes,
					unsigned char count,
					bool useLEDs,
					IButtonHW::ButtonState changeOnEvent) {
	buttonIndexes_ = buttonIndexes;
	buttonCount_ = count;
	changeOnEvent_ = changeOnEvent;
	useLEDs_ = useLEDs;
	ignoreButton_ = count;
}

void Switches::computeIgnoreButton() {
	for (unsigned char i = 0; i < buttonCount_; i++) {
		if (LEDsAndButtonsHWWrapper::isButtonDown(buttonIndexes_[i])) {
			ignoreButton_ = i;
		}
	}
}

void Switches::update() {
	for (unsigned char i = 0; i < buttonCount_; i++) {
		bool buttonDown = LEDsAndButtonsHWWrapper::isButtonDown(buttonIndexes_[i]);
		#ifdef DEBUG
		printf("Button %d(%d) %s\n", i, buttonIndexes_[i], buttonDown ? "down" : "up");
        #endif
        if (((changeOnEvent_ == IButtonHW::DOWN) && !GETBIT(lastStates_, i) && buttonDown) ||
        	((changeOnEvent_ == IButtonHW::UP) && GETBIT(lastStates_, i) && !buttonDown)	) {
            if (ignoreButton_ == i) {
            		ignoreButton_ = buttonCount_ + 1;
            } else {
        			setStatus(i, !GETBIT(statuses_, i));
            }
        }
        setBit(lastStates_, i, buttonDown);
    }
}

bool Switches::getStatus(unsigned char buttonIndex)
{
    return GETBIT(statuses_, buttonIndex) == 1;
}

void Switches::setStatus(unsigned char buttonIndex, bool value)
{
    if (value) {
        SETBITTRUE(statuses_, buttonIndex);
    } else {
        SETBITFALSE(statuses_, buttonIndex);
    }
    if (useLEDs_) {
    		LEDsAndButtonsHWWrapper::setLED(buttonIndexes_[buttonIndex], value ? ILEDHW::ON : ILEDHW::OFF);
    }
}
