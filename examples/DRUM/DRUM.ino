// libraries
#include <MIDI.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>
#include <trinityHW.h>
#include <MozziGuts.h>
#include <mozzi_rand.h>
#include <mozzi_analog.h>
//#include <LowPassFilter.h>
#include <bastlSample.h>

#include <bastlSamples/kick.h>
#include <bastlSamples/snare.h> 
#include <bastlSamples/hat.h> 
#include <bastlSamples/CB.h> 
//#include <samples/ride.h>
//#include <bastlSamples/tom.h>


//#include <tables/whitenoise2048_int8.h>
//#include <tables/square_no_alias512_int8.h>
//#include <python/hat.h>

unsigned char inputChannel;
const char* WAVE_TABLES[]={
  KICK_DATA, SNARE_DATA, HAT_DATA, CB_DATA};//SQUARE_NO_ALIAS512_DATA};//HAT_DATA};//WHITENOISE2048_DATA};

const unsigned int WAVE_TABLES_NUM_CELLS[]={
  KICK_NUM_CELLS, SNARE_NUM_CELLS, HAT_NUM_CELLS, CB_NUM_CELLS};

#define NUMBER_OF_WAVETABLES 4

#define NUMBER_OF_VOICES 3

#define CONTROL_RATE 64 // powers of 2 please
/*
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aOsc[NUMBER_OF_VOICES]={
 (SIN2048_DATA)};
 Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aMod[NUMBER_OF_VOICES]={
 (SIN2048_DATA)};
 */
//Sample<AUDIO_RATE> aSample[0](KICK_DATA,KICK_NUM_CELLS);
//Sample<AUDIO_RATE> aSample[1](KICK_DATA,KICK_NUM_CELLS);
//Sample<AUDIO_RATE> aSample[2](KICK_DATA,KICK_NUM_CELLS);
Sample<AUDIO_RATE> aSample[NUMBER_OF_VOICES];


trinityHW hw; // MOZZI or DEFAULT

void setup() {
  
  // debug();
  hw.setFreezeType(UNFREEZE_EXTERNALY);
  hw.initialize(DEFAULT);
  //Serial.begin(9600);

  animation();
  initMem();
  initMidi(getMidiChannel());
  
  startMozzi(CONTROL_RATE);

  //adcReconnectAllDigitalIns();
  //yhw.initialize(DEFAULT);
  for(int i=0;i<NUMBER_OF_VOICES;i++) aSample[i].setTable(WAVE_TABLES[i],WAVE_TABLES_NUM_CELLS[i]), aSample[i].setTimeStretchOn(), aSample[i].setTimeStretch(5);
  hw.update();
  hw.update();
}

void updateControl(){

  while(MIDI.read(inputChannel)) MIDI.read(inputChannel);
  hw.update();
  UI();
  while(MIDI.read(inputChannel)) MIDI.read(inputChannel);

}

void loop() {
  audioHook();
}



