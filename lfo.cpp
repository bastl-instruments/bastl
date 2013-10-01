/* 
	
lfo.cpp library
this library is to be used with 
MOZZI library compatible
documentation of the library you find here:
lfo library

Created by Vaclav Pelousek 2013 www.pelousek.eu
for Standuino wwww.standuino.eu

*/


#include "Arduino.h"
#include <avr/pgmspace.h>
#include <mozzi_rand.h>
#include "lfo.h"

#define UP_DOWN 0
#define UP 1
#define DOWN 2
#define FLIP 3
#define FLIP_UP_DOWN 4
#define JUMP_UP_DOWN 5
#define RANDOM_SMOOTH 6
#define RANDOM 7


lfo::lfo(){
}

void lfo::update(){
	counter++;
	switch(shape){
		case UP_DOWN:
			if(counter>=stepLenght) step++,counter=0;
			if(up){
			    value=stepSize*step;
				if(value>=MAX_VALUE) up=false, value=255, counter=0, step=0;
			}
		
			else{ 
				value=MAX_VALUE-stepSize*step;
				if(value<=MIN_VALUE) up=true, value=0, counter=0,step=0;
			}
			returnValue=value-128;
		break;
		
		case UP:
			
			if(counter>=stepLenght) step++,counter=0;
			value=stepSize*step;
			if(value>=MAX_VALUE) value=0, counter=0, step=0;
			returnValue=value;
		break;
		
		case DOWN:
			
			if(counter>=stepLenght) step++,counter=0;
			//value=MAX_VALUE-stepSize*step;
			//if(value<=MIN_VALUE) value=255, counter=0, step=0;
			value=stepSize*step;
			if(value>=MAX_VALUE) value=0, counter=0, step=0;
			returnValue=MIN_VALUE-value;
		break;
		
		case FLIP:
			if(counter>=stepLenght) step++,counter=0,up=!up;
			value=stepSize*step;
			if(value>=MAX_VALUE) value=0, counter=0, step=0;
			if(up) returnValue=value;
			else returnValue=MIN_VALUE-value;
		break;
		
		case FLIP_UP_DOWN:
			if(counter>=stepLenght) step++,counter=0,flip=!flip;
			
			if(up){
			    value=stepSize*step;
				if(value>=MAX_VALUE) up=false, value=255, counter=0, step=0;
			}
		
			else{ 
				value=MAX_VALUE-stepSize*step;
				if(value<=MIN_VALUE) up=true, value=0, counter=0,step=0;
			}
			if(flip) returnValue=value;
			else returnValue=MIN_VALUE-value;
		break;
		
		case JUMP_UP_DOWN:
			if(counter>=stepLenght) step++,counter=0,flip=!flip;
			
			if(up){
			    value=stepSize*step;
				if(value>=MAX_VALUE) up=false, value=255, counter=0, step=0;
			}
		
			else{ 
				value=MAX_VALUE-stepSize*step;
				if(value<=MIN_VALUE) up=true, value=0, counter=0,step=0;
			}
			if(flip) returnValue=value;
			else returnValue=0;
		break;
		
		case RANDOM_SMOOTH:
			if(counter>=(rate>>4)) {
				counter=0;
				if(rand(stepSize)==0) flip=!flip;
				if(flip) step+=stepSize;
				else step-=stepSize;
			}
			value=step;
			//value+=stepSize*(step-res);// value=rand(255);
			returnValue=value-128;
		break;
		case RANDOM:
			if(counter>=(rate>>4)) counter=0,value=stepSize*rand(res);
			returnValue=value-128;
			
		break;
	}
}

void lfo::setShape(unsigned char _SHAPE){
  	shape=_SHAPE;
}

void lfo::setResolution(unsigned char _RESOLUTION){
	res=_RESOLUTION+1;
	stepSize=MAX_VALUE/res;
	stepLenght=rate/res;
}

void lfo::reset(){
	up=true;
	flip=false;
	step=0;
	counter=0;
}

void lfo::setRate(unsigned char _RATE){
	rate=MAX_VALUE-_RATE+1;
}

void lfo::setAll(unsigned char _RATE,unsigned char _SHAPE,unsigned char _RESOLUTION){
	shape=_SHAPE;
	rate=MAX_VALUE-_RATE+1;
	res=_RESOLUTION+1;
	
	if(_SHAPE<RANDOM_SMOOTH)if(res>rate) res=rate;
	stepSize=MAX_VALUE/res;
	stepLenght=rate/res;
}
	
int lfo::next(){
 	return returnValue;
}


