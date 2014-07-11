#include "RadioButtons.h"

//#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

RadioButtons::RadioButtons(IHWLayer *hwLayer, unsigned char * buttonIndexes, unsigned char count) :
    hwLayer_(hwLayer), buttonIndexes_(buttonIndexes), buttonCount_(count), selectedButton_(0), buttonSelected_(false)
{

}

void RadioButtons::update() {
    for (unsigned char i = 0; i < buttonCount_; i++) {
        bool buttonDown = hwLayer_->buttonDown(buttonIndexes_[i]);
#ifdef DEBUG
        printf("Button: %i %s \n", buttonIndexes_[i], buttonDown ? "DOWN" : "UP");
#endif
        if (buttonDown) {
            if (buttonSelected_ && selectedButton_ == i)
            {
                buttonSelected_ = false;
            } else {
                buttonSelected_ = true;
                selectedButton_ = i;
            }
            break;
        }
    }
}


bool RadioButtons::getSelectedButton(unsigned char &selectedButton)
{
    if (buttonSelected_) {
        selectedButton = buttonIndexes_[selectedButton_];
        return true;
    }
    return false;
}
