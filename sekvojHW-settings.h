/*
 * sekvojHW-settings.h
 *
 *  Created on: 15.07.2014
 *      Author: user
 */

#ifndef SEKVOJHW_SETTINGS_H_
#define SEKVOJHW_SETTINGS_H_

// used pins
#define SHIFTREGISTER_SER C,5
#define SHIFTREGISTER_RCK B,1
#define SHIFTREGISTER_SRCK B,0

#define BUTTONCOL_0 C,1
#define BUTTONCOL_1 C,0
#define BUTTONCOL_2 D,7
#define BUTTONCOL_3 D,6

#define SS_SDCARD B,2
#define SS_RAM    D,2


// the frequency at which the leds are updated.
// when set too high, resources are wasted, when set too low flikering occurs
// the value is in hertz and can be set from 16 to 255
#define FREQ 50


// properties of the led blinking
// total defines the frequency of one blink cycle
// duty defines when the led state is switched
// the values are relative to the frequency, so when you want dimming instead of blinking, you will need to increase that value
//  -------
//        |
//        |
//        -------------
//  0    duty         total

#define blinkTotal 20
#define blinkDuty  10



#endif /* SEKVOJHW_SETTINGS_H_ */
