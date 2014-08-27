/* 
	
trinityHW.cpp library
this library is to be used with standuino fraAngelico hardware
MOZZI library compatible
documentation of the library you find here:
trinityHW library

Created by Vaclav Pelousek 2013 www.pelousek.eu
for Standuino wwww.standuino.eu

*/


#include "Arduino.h"
#include <avr/pgmspace.h>
#include "trinityHW.h"
//#include <MozziGuts.h>
#include <../Mozzi/mozzi_analog.h>



const unsigned char buttonPins[NUMBER_OF_BUTTONS] PROGMEM = {
							BIG_BUTTON_1_PIN,
							BIG_BUTTON_2_PIN,
							BIG_BUTTON_3_PIN,
							
							SMALL_BUTTON_1_PIN, 
							SMALL_BUTTON_2_PIN,
							
							EXTRA_BUTTON_1_PIN,
							EXTRA_BUTTON_2_PIN,
};


const unsigned char ledPins[NUMBER_OF_LEDS] PROGMEM = {
							LED_1_PIN,
							LED_2_PIN,
							LED_3_PIN,
							LED_R_PIN,
							LED_G_PIN,
							LED_B_PIN,
							
};

const unsigned char knobPins[NUMBER_OF_KNOBS] PROGMEM = {  KNOB_1_PIN,
                                                  KNOB_2_PIN, 
                                                  KNOB_3_PIN, 
};


const unsigned char colorBits[NUMBER_OF_COLORS] PROGMEM = {

BLACK_BITS, RED_BITS,GREEN_BITS,BLUE_BITS,YELLOW_BITS,MAGENTA_BITS,CIAN_BITS,WHITE_BITS

};

trinityHW::trinityHW(){
	KNOB_TOLERANCE=2;
}

//############# ROUTINE FUNCTIONS #############

void trinityHW::initialize(unsigned char _HW_VERSION){ 

	for(int i=ZERO;i<NUMBER_OF_BUTTONS;i++) pinMode(pgm_read_word_near(buttonPins + i), INPUT_PULLUP);
	for(int i=ZERO;i<NUMBER_OF_LEDS;i++) pinMode(pgm_read_word_near(ledPins + i), OUTPUT);
	
	freezeAllKnobs();
	update();
	update();
}

void trinityHW::update(){
	updateKnobs();
	updateButtons(); 
	writeToLeds();
}







//############# KNOB RELATED FUNCTIONS #############

void trinityHW::setFreezeType(unsigned char _TYPE){

	if(_TYPE==DEFAULT) unfreezeExternaly=false;
	else if(_TYPE==UNFREEZE_EXTERNALY) unfreezeExternaly=true;

}

//update values and hashes of knobs
void trinityHW::updateKnobs(){

 knobChangedHash = ZERO;
  for (int i = ZERO; i < NUMBER_OF_KNOBS; i++) {
  
    short newValue = mozziAnalogRead(pgm_read_word_near(knobPins + i));
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

void trinityHW::setKnobTolerance(unsigned char _tolerance){
	KNOB_TOLERANCE=_tolerance;
}

//returns the freezing state of knob	
boolean trinityHW::knobFreezed(unsigned char _KNOB){ 
	return bitRead(knobFreezedHash,_KNOB);
}

//returns the freezing state of knob	
boolean trinityHW::knobMoved(unsigned char _KNOB){ 
	return bitRead(knobChangedHash,_KNOB);
}

// freeze all knobs
void trinityHW::freezeAllKnobs(){ 
	for(int i=ZERO;i<NUMBER_OF_KNOBS;i++){
		bitWrite(knobFreezedHash,i,true);
	}
}

// unfreeze all knobs
void trinityHW::unfreezeAllKnobs(){ 
	for(int i=ZERO;i<NUMBER_OF_KNOBS;i++){
		bitWrite(knobFreezedHash,i,false);
	}
}


// freeze one knob
void trinityHW::freezeKnob(unsigned char _KNOB){ 
	bitWrite(knobFreezedHash,_KNOB,true);
}

// unfreeze one knob
void trinityHW::unfreezeKnob(unsigned char _KNOB){ 
		bitWrite(knobFreezedHash,_KNOB,false);
}


// get knob value
int trinityHW::knobValue(unsigned char _KNOB){ 

	return knobValues[_KNOB];

}

// get last knob value
int trinityHW::lastKnobValue(unsigned char _KNOB){ 

	return lastKnobValues[_KNOB];

}

//############# LED RELATED FUNCTIONS #############

// write the values from the hash to the pins
void trinityHW::writeToLeds(){ 

	for(int i=ZERO;i<NUMBER_OF_LEDS;i++){
			//pinMode(pgm_read_word_near(ledPins + i), OUTPUT);
			digitalWrite(pgm_read_word_near(ledPins + i), !bitRead(ledStateHash,i));	
	}
 
}

void trinityHW::setColor(unsigned char _COLOR){ 

	unsigned char _bits=pgm_read_word_near(colorBits + _COLOR)	;
	
	for(int i=LED_R;i<=LED_B;i++){
		setLed(i,bitRead(_bits,i-LED_R));
	}

}
// set state of led
void trinityHW::setLed(unsigned char _LED,boolean _STATE){ 

	bitWrite(ledStateHash,_LED,_STATE);

}

//############# BUTTON RELATED FUNCTIONS #############

boolean trinityHW::mozziDigitalRead(unsigned char _pin){
   if(_pin>13) return mozziAnalogRead(_pin)>>9;
   else return digitalRead(_pin);
}

// updates all button related hashes
void trinityHW::updateButtons(){  

	for(int i=ZERO;i<NUMBER_OF_BUTTONS;i++){ // first read the buttons and update button states
	
		//	pinMode(pgm_read_word_near(buttonPins + i), INPUT_PULLUP);
		bitWrite(buttonStateHash,i,!mozziDigitalRead(pgm_read_word_near(buttonPins + i)));
		
		// and now update all the other hashes
	 	bitWrite(justPressedHash,i,false); 
		bitWrite(justReleasedHash,i,false);
    	if(bitRead(buttonStateHash,i)==true && bitRead(lastButtonStateHash,i)==false)  bitWrite(justPressedHash,i,true);
    	if(bitRead(buttonStateHash,i)==false && bitRead(lastButtonStateHash,i)==true)  bitWrite(justReleasedHash,i,true);
		bitWrite(lastButtonStateHash,i,bitRead(buttonStateHash,i));  
	}
 
}


//returns current state of a button
boolean trinityHW::buttonState(unsigned char _BUTTON){ 
	return bitRead(buttonStateHash,_BUTTON);
}

//returns true if the button was just pressed
boolean trinityHW::justPressed(unsigned char _BUTTON){

	return bitRead(justPressedHash,_BUTTON);

}

//returns true if the button was just released
boolean trinityHW::justReleased(unsigned char _BUTTON){
	return bitRead(justReleasedHash,_BUTTON);
}



//flips the software switch
void trinityHW::flipSwitch(unsigned char _SWITCH){ 
	bitWrite(switchStateHash,_SWITCH,!bitRead(switchStateHash,_SWITCH));
}

// sets switch state
void trinityHW::setSwitch(unsigned char _SWITCH, boolean _STATE){ 
	bitWrite(switchStateHash,_SWITCH,_STATE);
}

//returns switch state
boolean trinityHW::switchState(unsigned char _SWITCH){ 
	return bitRead(switchStateHash,_SWITCH);
}

//resetsSwitches
void trinityHW::resetSwitches(){
	for(int i=ZERO;i<NUMBER_OF_BUTTONS;i++){
		bitWrite(switchStateHash,i,false);
	}
}

//use switch states as bits of one number - sound
unsigned char trinityHW::soundFromSwitches(){
	unsigned char val=ZERO;
	for(int i=ZERO;i<3;i++){
		bitWrite(val,i,bitRead(switchStateHash,i));
	}
	return val;

}

//use button states as bits of one number - sound
unsigned char trinityHW::soundFromButtons(){
	unsigned char val=ZERO;
	for(int i=ZERO;i<NUMBER_OF_BIG_BUTTONS;i++){
		bitWrite(val,i,bitRead(buttonStateHash,i));
	}
	return val;
}


#define CLEAR_TIMES 8
#define CLEAR_DELAY 50
boolean trinityHW::factoryClear(){
	
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

void trinityHW::factoryCleared(){
	for(int i=ZERO;i<CLEAR_TIMES;i++){
		  	digitalWrite(FACTORY_CLEAR_SIGNAL_PIN,HIGH);
		  	delay(CLEAR_DELAY);
	  		digitalWrite(FACTORY_CLEAR_SIGNAL_PIN,LOW);
	  		delay(CLEAR_DELAY);
  		}
}
