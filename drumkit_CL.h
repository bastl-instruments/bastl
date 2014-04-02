/* 
	
lfo.cpp library
this library is to be used with standuino fraAngelico hardware
MOZZI library compatible
documentation of the library you find here:
lfo library

Created by Vaclav Pelousek 2013 www.pelousek.eu
for Standuino wwww.standuino.eu

*/

#ifndef drumkit_TR_h
#define drumkit_TR_h

#include <bastlSamples/KICK2.h>
#include <bastlSamples/SNARE2.h> 
#include <bastlSamples/HAT2.h> 
#include <bastlSamples/CB4.h> 
#include <bastlTables/haluz2048_int8.h> 

const char* WAVE_TABLES[]={
  KICK2_DATA, SNARE2_DATA, HAT2_DATA,CB4_DATA,HALUZ2048_DATA};

const unsigned int WAVE_TABLES_NUM_CELLS[]={
  KICK2_NUM_CELLS, SNARE2_NUM_CELLS, HAT2_NUM_CELLS, CB4_NUM_CELLS,HALUZ2048_NUM_CELLS};
#define NUMBER_OF_WAVETABLES 6


#endif