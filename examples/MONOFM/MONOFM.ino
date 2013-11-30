// libraries
#include <MIDI.h>
#include <EEPROM.h>
#include <trinityHW.h>
#include <MozziGuts.h>
#include <mozzi_rand.h>
#include <mozzi_midi.h>
#include <avr/pgmspace.h>
#include <lfo.h>

// mozzi objects
#include <Oscil.h>


// tables / samples
#include <tables/sin2048_int8.h> // 
#include <tables/saw2048_int8.h> // 
#include <tables/triangle2048_int8.h> // 
#include <bastlTables/whitenoise2048_int8.h>
unsigned char inputChannel;
boolean test;
const char* WAVE_TABLES[]={
  SIN2048_DATA, SAW2048_DATA, WHITENOISE2048_DATA};//TRIANGLE2048_DATA TRIANGLE_DIST_CUBED_2048_DATA};

#define NUMBER_OF_WAVETABLES 3
#define NUMBER_OF_LFO_SHAPES 8

#define NUMBER_OF_MODULATORS 3
#define MOD_1 0
#define MOD_2 1
#define MOD_3 2

#define CONTROL_RATE 64 // powers of 2 please
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aCar(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aMod[NUMBER_OF_MODULATORS]={
  (SIN2048_DATA),(SIN2048_DATA),(SIN2048_DATA)};
lfo LFO;


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
  hw.update();
  hw.update();
  checkForPokemon();
}

void updateControl(){

  while(MIDI.read(inputChannel)) MIDI.read(inputChannel);
  MIDI.read(inputChannel);
  MIDI.read(inputChannel);
  MIDI.read(inputChannel);
  MIDI.read(inputChannel);
  MIDI.read(inputChannel);
  hw.update();
  while(MIDI.read(inputChannel)) MIDI.read(inputChannel);
   MIDI.read();
  MIDI.read();
  MIDI.read();
  MIDI.read();
  MIDI.read();
  UI();
  while(MIDI.read(inputChannel)) MIDI.read(inputChannel);
   MIDI.read(inputChannel);
  MIDI.read(inputChannel);
  MIDI.read(inputChannel);
  MIDI.read(inputChannel);
  MIDI.read(inputChannel);

}

void loop() {
  audioHook();
}

































