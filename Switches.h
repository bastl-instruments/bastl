
#ifndef SWITCHES_H
#define SWITCHES_H

#include "IButtonHW.h"

/**
 * This class handles maximum 8 independent switches
 */
class Switches
{
public:

	/**
	 * Constructor
	 */
	Switches();

	/**
	 * Initialize switches
	 * @param hwLayer HW layer to read button states from
	 * @param buttonIndexes indexes of the buttons in HW layer to be used in this
	 * 					    button switches array (MAXIMUM IS 16)
	 * @param count number of buttons in this button array
	 * @param changeOnEvent event to change state on (Default is on button down)
	 */
    void init(IButtonHW *hwLayer, unsigned char * buttonIndexes, unsigned char count, IButtonHW::ButtonState changeOnEvent = IButtonHW::DOWN);

    /**
     * get the status of the button at the index in the switches array
     * @param buttonIndex index of the button in the switches array to bet value for
     * @return status of the button specified by argument
     */
    bool getStatus(unsigned char buttonIndex);

    /**
     * sets the status of the button at the index in the switches array
     * @param buttonIndex index of the button in the switches array to bet value for
     * @param value status to be set to the button specified by argument
     */
    void setStatus(unsigned char buttonIndex, bool value);

    /**
     * Updates status of all buttons reading values from the HW
     */
    void update();

private:
    IButtonHW * hwLayer_;
    unsigned char * buttonIndexes_;
    unsigned char buttonCount_;
    unsigned int lastStates_;
    unsigned int statuses_;
    unsigned char changeOnEvent_;
};

#endif // SWITCHES_H
