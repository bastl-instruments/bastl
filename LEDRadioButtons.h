#ifndef LEDRADIOBUTTONS_H
#define LEDRADIOBUTTONS_H


#include "ILEDsAndButtonsHW.h"
#include "RadioButtons.h"

class LEDRadioButtons : public RadioButtons
{
public:
	virtual void init(unsigned char * buttonIndexes, unsigned char count);
	virtual void setSelectedButton(unsigned char selectedButton);
    virtual void resetSelection();
};

#endif // LEDRADIOBUTTONS_H
