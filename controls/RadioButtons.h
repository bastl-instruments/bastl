#ifndef RADIOBUTTONS_H
#define RADIOBUTTONS_H


#include "../hw/IHWLayer.h"

class RadioButtons
{
public:
    RadioButtons(IHWLayer * hwLayer, unsigned char * buttonIndexes, unsigned char count);
    bool getSelectedButton(unsigned char & selectedButton);
    void setSelectedButton(unsigned char selectedButton);
    void update();

private:
    IHWLayer * hwLayer_;
    unsigned char * buttonIndexes_;
    unsigned char buttonCount_;
    unsigned char selectedButton_;
    unsigned char lastDownButton_;
    bool buttonSelected_;
};

#endif // RADIOBUTTONS_H
