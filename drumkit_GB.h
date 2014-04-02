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

#include <bastlSamples/GB_KICK.h>
#include <bastlSamples/GB_SNARE.h> 
#include <bastlSamples/GB_HH.h> 
#include <bastlSamples/GB_TUI.h> 
#include <bastlTables/haluz2048_int8.h> 
//#include <bastlSamples/GB_BLIP.h> 
//#include <bastlSamples/GB_BZZ.h> 

const char* WAVE_TABLES[]={
  GB_KICK_DATA, GB_SNARE_DATA, GB_HH_DATA,GB_TUI_DATA,HALUZ2048_DATA};//,GB_BZZ_DATA};

const unsigned int WAVE_TABLES_NUM_CELLS[]={
  GB_KICK_NUM_CELLS, GB_SNARE_NUM_CELLS, GB_HH_NUM_CELLS,GB_TUI_NUM_CELLS,HALUZ2048_NUM_CELLS};//,GB_BZZ_NUM_CELLS};
#define NUMBER_OF_WAVETABLES 5


#endif