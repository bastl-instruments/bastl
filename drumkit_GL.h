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

#include <bastlSamples/GL_A.h>
//#include <bastlSamples/GL_B.h>
#include <bastlSamples/GL_C.h>
#include <bastlSamples/GL_D.h>
#include <bastlSamples/GL_E.h>
#include <bastlSamples/GL_F.h>
#include <bastlSamples/GL_G.h>
#include <bastlSamples/GL_H.h>
#include <bastlSamples/GL_I.h>
 
#include <bastlTables/haluz2048_int8.h> 
//#include <bastlSamples/GB_BLIP.h> 
//#include <bastlSamples/GB_BZZ.h> 

const char* WAVE_TABLES[]={
  GL_D_DATA, GL_C_DATA, GL_A_DATA, GL_I_DATA,   GL_E_DATA, GL_F_DATA, GL_G_DATA, GL_H_DATA};//, GL_I_DATA, };//,GB_BZZ_DATA};

const unsigned int WAVE_TABLES_NUM_CELLS[]={
   GL_D_NUM_CELLS, GL_C_NUM_CELLS, GL_A_NUM_CELLS, GL_I_NUM_CELLS,  GL_E_NUM_CELLS, GL_F_NUM_CELLS, GL_G_NUM_CELLS, GL_H_NUM_CELLS};//, GL_I_NUM_CELLS, };//,GB_BZZ_NUM_CELLS};
#define NUMBER_OF_WAVETABLES 8


#endif