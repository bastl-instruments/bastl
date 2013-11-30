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
boolean test;
//#include <bastlSamples/kick.h>
#include <bastlSamples/KICK2.h>
#include <bastlSamples/SNARE2.h> 
//#include <bastlSamples/snare.h> 
//#include <bastlSamples/hat.h> 
#include <bastlSamples/HAT2.h> 
#include <bastlTables/haluz2048_int8.h> 
//#include <bastlSamples/CB.h> 
#include <bastlSamples/CB3.h> 
#include <bastlSamples/RIDE.h> 
//#include <samples/ride.h>
//#include <bastlSamples/tom.h>


//#include <tables/whitenoise2048_int8.h>
//#include <tables/square_no_alias512_int8.h>
//#include <python/hat.h>

unsigned char inputChannel;
const char* WAVE_TABLES[]={
  KICK2_DATA, SNARE2_DATA, HAT2_DATA,CB3_DATA,RIDE_DATA, HALUZ2048_DATA};//, };//SQUARE_NO_ALIAS512_DATA};//HAT_DATA};//WHITENOISE2048_DATA};

const unsigned int WAVE_TABLES_NUM_CELLS[]={
  KICK2_NUM_CELLS, SNARE2_NUM_CELLS, HAT2_NUM_CELLS, CB3_NUM_CELLS,RIDE_NUM_CELLS,HALUZ2048_NUM_CELLS};//, CB_NUM_CELLS};

#define NUMBER_OF_WAVETABLES 6

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
boolean bootShift=false;
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
  bootShift=hw.buttonState(SMALL_BUTTON_1);
}

void updateControl(){

  while(MIDI.read(inputChannel)) MIDI.read(inputChannel);
  while(MIDI.read()) MIDI.read();
  MIDI.read();
  MIDI.read();
  MIDI.read();
  MIDI.read();
  MIDI.read();

  hw.update();
  UI();
  while(MIDI.read(inputChannel)) MIDI.read(inputChannel);
  while(MIDI.read()) MIDI.read();
  MIDI.read();
  MIDI.read();
  MIDI.read();
  MIDI.read();
  MIDI.read();

}

void loop() {
  audioHook();
}




