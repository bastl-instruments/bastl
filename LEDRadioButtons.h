#ifndef LEDRADIOBUTTONS_H
#define LEDRADIOBUTTONS_H


#include "ILEDsAndButtonsHW.h"
#include "RadioButtons.h"

class LEDRadioButtons : public RadioButtons
{
public:
    LEDRadioButtons(ILEDsAndButtonsHW * hwLayer, unsigned char * buttonIndexes, unsigned char count);
    virtual ~LEDRadioButtons() {}
    virtual void setSelectedButton(unsigned char selectedButton);
    virtual void resetSelection();

protected:
    ILEDsAndButtonsHW * ledHWLayer_;
};

#endif // LEDRADIOBUTTONS_H
