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

#include <bastlSamples/TR_KICK.h>
#include <bastlSamples/TR_SNARE.h> 
#include <bastlSamples/TR_HH.h> 
#include <bastlSamples/TR_CB.h> 
#include <bastlSamples/TR_TOM.h> 
#include <bastlSamples/TR_CLAP.h> 

#include <bastlTables/haluz2048_int8.h> 

const char* WAVE_TABLES[]={
  TR_KICK_DATA, TR_SNARE_DATA, TR_HH_DATA,   TR_CB_DATA, TR_CLAP_DATA , TR_TOM_DATA}; //TR_OH_DATA, // TR_RIM_DATA,

const unsigned int WAVE_TABLES_NUM_CELLS[]={
  TR_KICK_NUM_CELLS, TR_SNARE_NUM_CELLS, TR_HH_NUM_CELLS,   TR_CB_NUM_CELLS, TR_CLAP_NUM_CELLS , TR_TOM_NUM_CELLS};//TR_OH_NUM_CELLS, //, TR_TOM_NUM_CELLS TR_RIM_NUM_CELLS,
#define NUMBER_OF_WAVETABLES 6


#endif