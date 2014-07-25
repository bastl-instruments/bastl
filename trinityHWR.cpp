/* 
	
trinityHWR.cpp library
this library is to be used with standuino fraAngelico hardware
MOZZI library compatible
documentation of the library you find here:
trinityHWR library

Created by Vaclav Pelousek 2013 www.pelousek.eu
for Standuino wwww.standuino.eu

*/


#include "Arduino.h"
#include <avr/pgmspace.h>
#include "trinityHWR.h"
//#include <MozziGuts.h>
#include <mozzi_analog.h>



prog_uchar buttonPinsR[NUMBER_OF_BUTTONS] PROGMEM = { 
							BIG_BUTTON_1_PIN,
							BIG_BUTTON_2_PIN,
							BIG_BUTTON_3_PIN,
							
							SMALL_BUTTON_1_PIN, 
							SMALL_BUTTON_2_PIN,
							
							EXTRA_BUTTON_1_PIN,
							EXTRA_BUTTON_2_PIN,
};


prog_uchar ledPinsR[NUMBER_OF_LEDS] PROGMEM = { 
							LED_1_PIN,
							LED_2_PIN,
							LED_3_PIN,
							LED_R_PIN,
							LED_G_PIN,
							LED_B_PIN,
							
};

prog_uchar knobPinsR[NUMBER_OF_KNOBS] PROGMEM = {  KNOB_1_PIN, 
                                                  KNOB_2_PIN, 
                                                  KNOB_3_PIN, 
};


prog_uchar colorBitsR[NUMBER_OF_COLORS] PROGMEM = {  

BLACK_BITS, RED_BITS,GREEN_BITS,BLUE_BITS,YELLOW_BITS,MAGENTA_BITS,CIAN_BITS,WHITE_BITS

};

trinityHWR::trinityHWR(){
	KNOB_TOLERANCE=2;
}

//############# ROUTINE FUNCTIONS #############

void trinityHWR::initialize(unsigned char _HW_VERSION){ 

	for(int i=ZERO;i<NUMBER_OF_BUTTONS;i++) pinMode(pgm_read_word_near(buttonPinsR + i), INPUT_PULLUP);
	for(int i=ZERO;i<NUMBER_OF_LEDS;i++) pinMode(pgm_read_word_near(ledPinsR + i), OUTPUT);
	
	freezeAllKnobs();
	update();
	update();
}

void trinityHWR::update(){
	updateKnobs();
	updateButtons(); 
	writeToLeds();
}







//############# KNOB RELATED FUNCTIONS #############

void trinityHWR::setFreezeType(unsigned char _TYPE){

	if(_TYPE==DEFAULT) unfreezeExternaly=false;
	else if(_TYPE==UNFREEZE_EXTERNALY) unfreezeExternaly=true;

}

//update values and hashes of knobs
void trinityHWR::updateKnobs(){

 knobChangedHash = ZERO;
  for (int i = ZERO; i < NUMBER_OF_KNOBS; i++) {
  
    short newValue = 1023 - mozziAnalogRead(pgm_read_word_near(knobPinsR + i));
    short distance = abs(newValue - knobValues[i]); 
    
    
    if(!unfreezeExternaly){
    	if (bitRead(knobFreezedHash, i) == true) {
    	  if (distance > KNOB_FREEZE_DISTANCE) {
     	   bitWrite(knobFreezedHash, i, false);
     	   bitWrite(knobChangedHash, i, true);
    	  }
  	  } 
    }
    
   	if (abs(newValue - knobValues[i]) > KNOB_TOLERANCE) bitWrite(knobChangedHash, i, true), activity=ZERO;
    else {
    
    if(activity>ACTIVITY_LIMIT) bitWrite(knobChangedHash, i, false);
    else activity++;
    
    }
    
    lastKnobValues[i]=knobValues[i];
    knobValues[i] = newValue;
  }
  
}

void trinityHWR::setKnobTolerance(unsigned char _tolerance){
	KNOB_TOLERANCE=_tolerance;
}

//returns the freezing state of knob	
boolean trinityHWR::knobFreezed(unsigned char _KNOB){ 
	return bitRead(knobFreezedHash,_KNOB);
}

//returns the freezing state of knob	
boolean trinityHWR::knobMoved(unsigned char _KNOB){ 
	return bitRead(knobChangedHash,_KNOB);
}

// freeze all knobs
void trinityHWR::freezeAllKnobs(){ 
	for(int i=ZERO;i<NUMBER_OF_KNOBS;i++){
		bitWrite(knobFreezedHash,i,true);
	}
}

// unfreeze all knobs
void trinityHWR::unfreezeAllKnobs(){ 
	for(int i=ZERO;i<NUMBER_OF_KNOBS;i++){
		bitWrite(knobFreezedHash,i,false);
	}
}


// freeze one knob
void trinityHWR::freezeKnob(unsigned char _KNOB){ 
	bitWrite(knobFreezedHash,_KNOB,true);
}

// unfreeze one knob
void trinityHWR::unfreezeKnob(unsigned char _KNOB){ 
		bitWrite(knobFreezedHash,_KNOB,false);
}


// get knob value
int trinityHWR::knobValue(unsigned char _KNOB){ 

	return knobValues[_KNOB];

}

// get last knob value
int trinityHWR::lastKnobValue(unsigned char _KNOB){ 

	return lastKnobValues[_KNOB];

}

//############# LED RELATED FUNCTIONS #############

// write the values from the hash to the pins
void trinityHWR::writeToLeds(){ 
	if(independentRGB){
		for(int i=ZERO;i<3;i++){
			digitalWrite(pgm_read_word_near(ledPinsR + i), !bitRead(ledStateHash,i));
		}
	}
	else{
	for(int i=ZERO;i<NUMBER_OF_LEDS;i++){
			//pinMode(pgm_read_word_near(ledPinsR + i), OUTPUT);
			digitalWrite(pgm_read_word_near(ledPinsR + i), !bitRead(ledStateHash,i));	
	}
 }
}

void trinityHWR::setColor(unsigned char _COLOR){ 

	unsigned char _bits=pgm_read_word_near(colorBitsR + _COLOR)	;
	
	for(int i=LED_R;i<=LED_B;i++){
		setLed(i,bitRead(_bits,i-LED_R));
	}

}
// set state of led
void trinityHWR::setLed(unsigned char _LED,boolean _STATE){ 

	bitWrite(ledStateHash,_LED,_STATE);

}

void trinityHWR::setIndependentRGBState(boolean _STATE){ 

	independentRGB = _STATE;

}
//############# BUTTON RELATED FUNCTIONS #############

boolean trinityHWR::mozziDigitalRead(unsigned char _pin){
   if(_pin>13) return mozziAnalogRead(_pin)>>9;
   else return digitalRead(_pin);
}

// updates all button related hashes
void trinityHWR::updateButtons(){  

	for(int i=ZERO;i<NUMBER_OF_BUTTONS;i++){ // first read the buttons and update button states
	
		//	pinMode(pgm_read_word_near(buttonPinsR + i), INPUT_PULLUP);
		bitWrite(buttonStateHash,i,!mozziDigitalRead(pgm_read_word_near(buttonPinsR + i)));
		
		// and now update all the other hashes
	 	bitWrite(justPressedHash,i,false); 
		bitWrite(justReleasedHash,i,false);
    	if(bitRead(buttonStateHash,i)==true && bitRead(lastButtonStateHash,i)==false)  bitWrite(justPressedHash,i,true);
    	if(bitRead(buttonStateHash,i)==false && bitRead(lastButtonStateHash,i)==true)  bitWrite(justReleasedHash,i,true);
		bitWrite(lastButtonStateHash,i,bitRead(buttonStateHash,i));  
	}
 
}


//returns current state of a button
boolean trinityHWR::buttonState(unsigned char _BUTTON){ 
	return bitRead(buttonStateHash,_BUTTON);
}

//returns true if the button was just pressed
boolean trinityHWR::justPressed(unsigned char _BUTTON){

	return bitRead(justPressedHash,_BUTTON);

}

//returns true if the button was just released
boolean trinityHWR::justReleased(unsigned char _BUTTON){
	return bitRead(justReleasedHash,_BUTTON);
}



//flips the software switch
void trinityHWR::flipSwitch(unsigned char _SWITCH){ 
	bitWrite(switchStateHash,_SWITCH,!bitRead(switchStateHash,_SWITCH));
}

// sets switch state
void trinityHWR::setSwitch(unsigned char _SWITCH, boolean _STATE){ 
	bitWrite(switchStateHash,_SWITCH,_STATE);
}

//returns switch state
boolean trinityHWR::switchState(unsigned char _SWITCH){ 
	return bitRead(switchStateHash,_SWITCH);
}

//resetsSwitches
void trinityHWR::resetSwitches(){
	for(int i=ZERO;i<NUMBER_OF_BUTTONS;i++){
		bitWrite(switchStateHash,i,false);
	}
}

//use switch states as bits of one number - sound
unsigned char trinityHWR::soundFromSwitches(){
	unsigned char val=ZERO;
	for(int i=ZERO;i<3;i++){
		bitWrite(val,i,bitRead(switchStateHash,i));
	}
	return val;

}

//use button states as bits of one number - sound
unsigned char trinityHWR::soundFromButtons(){
	unsigned char val=ZERO;
	for(int i=ZERO;i<NUMBER_OF_BIG_BUTTONS;i++){
		bitWrite(val,i,bitRead(buttonStateHash,i));
	}
	return val;
}


#define CLEAR_TIMES 8
#define CLEAR_DELAY 50
boolean trinityHWR::factoryClear(){
	
	pinMode(FACTORY_CLEAR_PIN,INPUT_PULLUP);
	pinMode(FACTORY_CLEAR_SIGNAL_PIN,OUTPUT);
	if(!digitalRead(FACTORY_CLEAR_PIN)){	 
 		for(int i=ZERO;i<CLEAR_TIMES;i++){
		  digitalWrite(FACTORY_CLEAR_SIGNAL_PIN,HIGH);
		  delay(CLEAR_DELAY);
		 digitalWrite(FACTORY_CLEAR_SIGNAL_PIN,LOW);
		  delay(CLEAR_DELAY);
  		}
  	return true;
	}
  else return false;
	 
}

void trinityHWR::factoryCleared(){
	for(int i=ZERO;i<CLEAR_TIMES;i++){
		  	digitalWrite(FACTORY_CLEAR_SIGNAL_PIN,HIGH);
		  	delay(CLEAR_DELAY);
	  		digitalWrite(FACTORY_CLEAR_SIGNAL_PIN,LOW);
	  		delay(CLEAR_DELAY);
  		}
}