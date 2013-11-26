/* 
	
envelope.cpp library
this library is to be used with 
MOZZI library compatible
documentation of the library you find here:
envelope library

Created by Vaclav Pelousek 2013 www.pelousek.eu
for Standuino wwww.standuino.eu

*/


#include "Arduino.h"
#include <avr/pgmspace.h>
#include "envelope.h"




envelope::envelope(){
}

void envelope::update(){ 
	if(_active){
		switch(phase){
			case ATTACK_PHASE:
			
				counter++;
				value=counter*attackStep;
				if(value>=MAX_VALUE) phase=DECAY_PHASE, counter=0, value=MAX_VALUE;
			
			break;
			case DECAY_PHASE:
				counter++;
				value=MAX_VALUE-counter*decayStep;
				if(value<=activeSustain) phase=SUSTAIN_PHASE, counter=0, value=activeSustain;
			
			break;
			case SUSTAIN_PHASE:
				value=activeSustain;
				if(activeSustain==0) phase=END_PHASE, _active=false, counter=0, value=0;
			break;
			
			case RELEASE_PHASE:
				counter++;
				value=releaseFrom-counter*releaseStep;
				if(value<=MIN_VALUE) phase=END_PHASE, counter=0, _active=false, value=0;
			break;
			case END_PHASE:
			_active=false, value=0;
			break;
		
		}
	}
}
void envelope::setADSR(unsigned char _ATTACK,unsigned char _DECAY,unsigned char _SUSTAIN, unsigned char _RELEASE){
	attack=_ATTACK;
	decay=_DECAY;
	sustain=_SUSTAIN;
	release=_RELEASE;
	
	attackStep=255/attack;
	decayStep=(255-sustain)/decay;
	releaseStep=sustain/release;
}
void envelope::setAttack(unsigned char _ATTACK){
	attack=_ATTACK;
	attackStep=255/attack;
}
void envelope::setDecay(unsigned char _DECAY){
	decay=_DECAY;
	decayStep=(255-sustain)/decay;
}
void envelope::setSustain(unsigned char _SUSTAIN){
	sustain=_SUSTAIN;
	if(phase==SUSTAIN_PHASE) activeSustain=(sustain*velocity)>>7;
}
void envelope::setRelease(unsigned char _RELEASE){
	release=_RELEASE;
	releaseStep=sustain/release;
}
    
void envelope::noteOn(){
	value=0;
	counter=0;
	phase=ATTACK_PHASE;
	_active=true;
	MAX_VALUE=255;
	activeSustain=sustain;
}

void envelope::noteOn(unsigned char _VELOCITY){
	velocity=_VELOCITY;
	value=0;
	counter=0;
	phase=ATTACK_PHASE;
	_active=true;
	MAX_VALUE=velocity<<1;
	activeSustain=(sustain*velocity)>>7;
	decayStep=(255-activeSustain)/decay;
	
}
void envelope::noteOff(){
	if(phase==SUSTAIN_PHASE) releaseFrom=activeSustain, counter=0, phase=RELEASE_PHASE, releaseStep=releaseFrom/release;
	else releaseFrom=value, counter=0, phase=RELEASE_PHASE, releaseStep=releaseFrom/release;
	
	if(releaseStep==0) releaseStep=1;
}
    
unsigned char envelope::next(){
	return value;
}
unsigned char envelope::getPhase(){
	return phase;
}

boolean envelope::active(){
	return _active;
}


