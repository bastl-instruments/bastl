#include "Switches.h"

//#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

Switches::Switches(IHWLayer *hwLayer, unsigned char * buttonIndexes, unsigned char count, IHWLayer::ButtonState changeOnEvent) :
    hwLayer_(hwLayer),
    buttonIndexes_(buttonIndexes),
    buttonCount_(count),
    lastStates_(0),
    statuses_(0),
    changeOnEvent_(changeOnEvent)
{

}

void Switches::update() {
    for (unsigned char i = 0; i < buttonCount_; i++) {
        bool buttonDown = hwLayer_->getButtonState(buttonIndexes_[i]) == IHWLayer::DOWN;
        if (((changeOnEvent_ == IHWLayer::DOWN) && !GETBIT(lastStates_, i) && buttonDown) ||
        	((changeOnEvent_ == IHWLayer::UP) && GETBIT(lastStates_, i) && !buttonDown)	) {
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
