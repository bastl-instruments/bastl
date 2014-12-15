#include "Switches.h"
#include "BitArrayOperations.h"

//#define DEBUG

#ifdef DEBUG
#include <cstdio>
#endif

Switches::Switches() :
    hwLayer_(0),
    buttonIndexes_(0),
    buttonCount_(0),
    lastStates_(0),
    statuses_(0),
    changeOnEvent_(IButtonHW::DOWN)
{
}

void Switches::init(IButtonHW *hwLayer, unsigned char * buttonIndexes, unsigned char count, IButtonHW::ButtonState changeOnEvent) {
	hwLayer_ = hwLayer;
	buttonIndexes_ = buttonIndexes;
	buttonCount_ = count;
	changeOnEvent_ = changeOnEvent;
}

void Switches::update() {
    for (unsigned char i = 0; i < buttonCount_; i++) {
        bool buttonDown = hwLayer_->getButtonState(buttonIndexes_[i]) == IButtonHW::DOWN;
		#ifdef DEBUG
		printf("Button %d(%d) %s\n", i, buttonIndexes_[i], buttonDown ? "down" : "up");
        #endif
        if (((changeOnEvent_ == IButtonHW::DOWN) && !GETBIT(lastStates_, i) && buttonDown) ||
        	((changeOnEvent_ == IButtonHW::UP) && GETBIT(lastStates_, i) && !buttonDown)	) {
            if (GETBIT(statuses_, i)) {
                SETBITFALSE(statuses_, i);
            } else {
                SETBITTRUE(statuses_, i);
            }
        }
        if (buttonDown) {
            SETBITTRUE(lastStates_, i);
        } else {
            SETBITFALSE(lastStates_, i);
        }
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
}
