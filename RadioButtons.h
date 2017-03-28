#ifndef RADIOBUTTONS_H
#define RADIOBUTTONS_H


#include "IButtonHW.h"

class RadioButtons
{
public:
    virtual void init(IButtonHW * hwLayer, unsigned char * buttonIndexes, unsigned char count);
    bool getSelectedButton(unsigned char & selectedButton);
    virtual void setSelectedButton(unsigned char selectedButton);
    void update();
    virtual void resetSelection();

protected:
    IButtonHW * hwLayer_;
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
