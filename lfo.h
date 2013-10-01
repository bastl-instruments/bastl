/* 
	
lfo.cpp library
this library is to be used with standuino fraAngelico hardware
MOZZI library compatible
documentation of the library you find here:
lfo library

Created by Vaclav Pelousek 2013 www.pelousek.eu
for Standuino wwww.standuino.eu

*/

#ifndef lfo_h
#define lfo_h

#include "Arduino.h"
#include <avr/pgmspace.h>


#define MAX_VALUE 255
#define MIN_VALUE 0

class lfo
{
  public: 
  
  	lfo(); // constructor unsigned char CONTROL_RATE
  	void setShape(unsigned char _SHAPE);
  	void setResolution(unsigned char _RESOLUTION);
  	void reset();
  	void setRate(unsigned char _RATE);
  	void setAll(unsigned char _RATE,unsigned char _SHAPE,unsigned char _RESOLUTION);
	
	void update();
	int next();
	

  private: 
  
	int counter;
	int	 value;
	int returnValue;
	unsigned char res;
	unsigned char rate;
	unsigned char shape;
	unsigned char step;
	unsigned char stepLenght;
	unsigned char stepSize;
	
	boolean up;
	boolean flip;
	
	
};

#endif