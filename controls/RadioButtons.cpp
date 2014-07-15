#include "RadioButtons.h"

//#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

RadioButtons::RadioButtons(IHWLayer *hwLayer, unsigned char * buttonIndexes, unsigned char count) :
    hwLayer_(hwLayer), buttonIndexes_(buttonIndexes), buttonCount_(count), selectedButton_(0), lastDownButton_(255), buttonSelected_(false)
{

}

void RadioButtons::update() {
    for (unsigned char i = 0; i < buttonCount_; i++) {
        bool buttonDown = hwLayer_->getButtonState(buttonIndexes_[i]) == IHWLayer::DOWN;
#ifdef DEBUG
        printf("Button: %i %s Last button down: %i\n", buttonIndexes_[i], buttonDown ? "DOWN" : "UP", lastDownButton_);
#endif
        if (buttonDown) {
            //Button that is down is the same as previous one so just ignore everything
            if (lastDownButton_ == i) {
                return;
            }
            if (buttonSelected_ && selectedButton_ == i)
            {
                buttonSelected_ = false;
            } else {
                buttonSelected_ = true;
                selectedButton_ = i;
            }
            lastDownButton_ = i;
            return;
        }
        //Disable lastButtonDown by value that hopefully will never occur
    }
    lastDownButton_ = 255;
}


bool RadioButtons::getSelectedButton(unsigned char &selectedButton)
{
    if (buttonSelected_) {
        selectedButton = selectedButton_;
        return true;
    }
    return false;
}
