/* 
	
trinityHW.cpp library
this library is to be used with standuino fraAngelico hardware
MOZZI library compatible
documentation of the library you find here:
trinityHW library

Created by Vaclav Pelousek 2013 www.pelousek.eu
for Standuino wwww.standuino.eu

*/

#ifndef trinityHW_h
#define trinityHW_h

#include "Arduino.h"
#include <avr/pgmspace.h>
//#include <MozziGuts.h>


#define DEFAULT 0
#define UNFREEZE_EXTERNALY 1

#define FALSE 0
#define TRUE 1

#define NUMBER_OF_KNOBS 3
#define NUMBER_OF_BIG_BUTTONS 3
#define NUMBER_OF_BUTTONS 7
#define NUMBER_OF_LEDS 6


// pin definitions //

#define KNOB_1_PIN A1 
#define KNOB_2_PIN A2 
#define KNOB_3_PIN A4 //A3


#define BIG_BUTTON_1_PIN 12
#define BIG_BUTTON_2_PIN 13
#define BIG_BUTTON_3_PIN 8

#define LED_1_PIN 7
#define LED_2_PIN 6
#define LED_3_PIN 2


//TAYDA RGB
#define LED_R_PIN 5
#define LED_G_PIN 3
#define LED_B_PIN 4

/*
//GME RGB
#define LED_R_PIN 4
#define LED_G_PIN 5
#define LED_B_PIN 3
//
*/

#define ZERO 0

#define SMALL_BUTTON_1_PIN 11
#define SMALL_BUTTON_2_PIN 17 //18

#define EXTRA_BUTTON_1_PIN 14
#define EXTRA_BUTTON_2_PIN 19

#define FACTORY_CLEAR_PIN 2
#define FACTORY_CLEAR_SIGNAL_PIN 13

// logic definitions //


#define ACTIVITY_LIMIT 25
#define KNOB_FREEZE_DISTANCE 32

#define KNOB_1 0
#define KNOB_2 1
#define KNOB_3 2

#define KNOB_TOP 0
#define KNOB_LEFT 1
#define KNOB_RIGHT 2

#define TOP_KNOB 0
#define LEFT_KNOB 1
#define RIGHT_KNOB 2

#define BIG_BUTTON_1 0
#define BIG_BUTTON_2 1
#define BIG_BUTTON_3 2

#define BIG_BUTT_1 0
#define BIG_BUTT_2 1
#define BIG_BUTT_3 2

#define SMALL_BUTTON_1 3
#define SMALL_BUTTON_2 4

#define EXTRA_BUTTON_1 5
#define EXTRA_BUTTON_2 6

#define SMALL_BUTT_1 3
#define SMALL_BUTT_2 4

#define EXTRA_BUTT_1 5
#define EXTRA_BUTT_2 6

#define LED_1 0
#define LED_2 1
#define LED_3 2
#define LED_R 3
#define LED_G 4
#define LED_B 5


#define NUMBER_OF_COLORS 8

#define BLACK 0
#define RED 1
#define GREEN 2
#define BLUE 3
#define YELLOW 4
#define MAGENTA 5
#define CIAN 6
#define WHITE 7

#define BLACK_BITS 0
#define RED_BITS 1
#define GREEN_BITS 2
#define BLUE_BITS 4
#define YELLOW_BITS 3
#define MAGENTA_BITS 5
#define CIAN_BITS 6
#define WHITE_BITS 7




// extern int led; i can acces from the main


class trinityHW
{
  public: 
  
    trinityHW(); // constructor
	void initialize(unsigned char _HW_VERSION);
	void update();

    void setLed(unsigned char _LED, boolean _STATE);
    void setColor(unsigned char _COLOR);
    
    boolean buttonState(unsigned char _BUTTON); // LFO // LEARD // EDIT
	boolean justPressed(unsigned char _BUTTON);
	boolean justReleased(unsigned char _BUTTON);
	
	boolean knobFreezed(unsigned char _KNOB);
	boolean knobMoved(unsigned char _KNOB);
	void freezeAllKnobs();
	void unfreezeAllKnobs();
	void freezeKnob(unsigned char _KNOB);
	void unfreezeKnob(unsigned char _KNOB);
	int knobValue(unsigned char _KNOB);
	int lastKnobValue(unsigned char _KNOB);
	void setFreezeType(unsigned char _TYPE);
	
	void setKnobTolerance(unsigned char _tolerance);
	
	
	void flipSwitch(unsigned char _SWITCH);
	void setSwitch(unsigned char _SWITCH, boolean _STATE);
	boolean switchState(unsigned char _SWITCH);
	void resetSwitches();


	unsigned char soundFromSwitches();
	unsigned char soundFromButtons();
	
	boolean factoryClear();
	void factoryCleared();


  private: 
  unsigned char KNOB_TOLERANCE;
 	unsigned char activity;
	int buttonStateHash;
	int lastButtonStateHash;
	int switchStateHash;
	int justPressedHash;
	int justReleasedHash;
	unsigned char ledStateHash;
	unsigned char knobFreezedHash;
	unsigned char knobChangedHash;
	int knobValues[NUMBER_OF_KNOBS];
	int lastKnobValues[NUMBER_OF_KNOBS];
	boolean mozziDigitalRead(unsigned char _pin);
	
	void updateKnobs();
	void updateButtons();
	void writeToLeds();
	
	boolean mozzi;
	boolean unfreezeExternaly;

	
};

#endif