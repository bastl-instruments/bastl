#include "Switches.h"

//#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

Switches::Switches(IHWLayer *hwLayer, unsigned char * buttonIndexes, unsigned char count) :
    hwLayer_(hwLayer),
    buttonIndexes_(buttonIndexes),
    buttonCount_(count),
    lastStates_(0),
    changedStates_(0),
    statuses_(0)
{

}

void Switches::update() {
    for (unsigned char i = 0; i < buttonCount_; i++) {
        bool buttonDown = hwLayer_->getButtonState(buttonIndexes_[i]) == IHWLayer::DOWN;
        if (!GETBIT(lastStates_, i) && buttonDown) {
            SETBITTRUE(changedStates_, i);
            if (GETBIT(statuses_, i)) {
                SETBITFALSE(statuses_, i);
            } else {
                SETBITTRUE(statuses_, i);
            }
        } else {
            SETBITFALSE(changedStates_, i);
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
    return GETBIT(statuses_, buttonIndex);
}

void Switches::setStatus(unsigned char buttonIndex, bool value)
{
    if (value) {
        SETBITTRUE(statuses_, buttonIndex);
    } else {
        SETBITFALSE(statuses_, buttonIndex);
    }
}
