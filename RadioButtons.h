#ifndef RADIOBUTTONS_H
#define RADIOBUTTONS_H

#include "LEDsAndButtonsHWWrapper.h"

class RadioButtons
{
public:
    virtual void init(unsigned char * buttonIndexes, unsigned char count);
    bool getSelectedButton(unsigned char & selectedButton);
    virtual void setSelectedButton(unsigned char selectedButton);
    void update();
    virtual void resetSelection();

protected:
    unsigned char * buttonIndexes_;
    unsigned char buttonCount_;
    unsigned char selectedButton_;
    unsigned char lastDownButton_;
    bool buttonSelected_;
};

inline void RadioButtons::resetSelection() {
	buttonSelected_ = false;
}

#endif // RADIOBUTTONS_H
