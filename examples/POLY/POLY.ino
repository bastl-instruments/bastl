 // libraries
#include <MIDI.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>
#include <trinityHW.h>
#include <MozziGuts.h>
#include <mozzi_midi.h>
#include <mozzi_rand.h>
#include <mozzi_analog.h>
//#include <LowPassFilter.h>
#include <bastlStateVariable.h>
#include <envelope.h>  
#include <lfo.h>
#include <Oscil.h>

#include <tables/sin2048_int8.h>
#include <tables/saw2048_int8.h> 
#include <tables/square_analogue512_int8.h> 
#include <bastlTables/whitenoise2048_int8.h>

//#include <tables/whitenoise2048_int8.h>
//#include <tables/square_no_alias512_int8.h>
//#include <python/hat.h>
unsigned char inputChannel;

const char* WAVE_TABLES[]={
  SIN2048_DATA, SAW2048_DATA, SQUARE_ANALOGUE512_DATA,WHITENOISE2048_DATA};//SQUARE_NO_ALIAS512_DATA};//HAT_DATA};//WHITENOISE2048_DATA};

#define NUMBER_OF_WAVETABLES 4

#define NUMBER_OF_VOICES 3

#define CONTROL_RATE 64 // powers of 2 please
/*
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aOsc[NUMBER_OF_VOICES]={
 (SIN2048_DATA)};
 Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aMod[NUMBER_OF_VOICES]={
 (SIN2048_DATA)};
 */
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aOsc[NUMBER_OF_VOICES]={
  (SIN2048_DATA),(SIN2048_DATA),(SIN2048_DATA)};
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aMod[NUMBER_OF_VOICES]={
  (SIN2048_DATA),(SIN2048_DATA),(SIN2048_DATA)};

envelope ADSR[NUMBER_OF_VOICES];
lfo LFO[NUMBER_OF_VOICES];
StateVariable lpf[NUMBER_OF_VOICES];

trinityHW hw; // MOZZI or DEFAULT

void setup() {
  
  
  // debug();
  hw.setFreezeType(UNFREEZE_EXTERNALY);
  hw.initialize(DEFAULT);
  //Serial.begin(9600);
  //clearMemmory();
  //for(int i=0;i<1024;i++) EEPROM.write(i,0);
  animation();
  initMem();
  
  // setAllTables();
  initMidi(getMidiChannel());
  startMozzi(CONTROL_RATE);
  hw.update();
  hw.update();
  checkForPokemon();
}

void updateControl(){

  while(MIDI.read(inputChannel)) MIDI.read(inputChannel);
  hw.update();
  while(MIDI.read(inputChannel)) MIDI.read(inputChannel);
  UI();
  while(MIDI.read(inputChannel)) MIDI.read(inputChannel);


}

void loop() {
  audioHook();
}



