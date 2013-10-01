
#include <trinityHW.h>
#include <MIDI.h>
#include <EEPROM.h>
#include <sequencer.h>
#include <MozziGuts.h>
#include <bastlSample.h> // oscillator template
#include <bastlSamples/snare.h>
Sample <AUDIO_RATE> aSample;

#define CONTROL_RATE 512

/*
FraAngelicoHW library example
 */
trinityHW hw; // MOZZI or DEFAULT
sequencer seq(61); // set timeBase
//Memory mem;

void setup(){
  
  hw.setFreezeType(UNFREEZE_EXTERNALY);
  hw.initialize(DEFAULT);

  //Serial.begin(9600);
  //debug();
  aSample.setTable(SNARE_DATA,SNARE_NUM_CELLS);
  aSample.setFreq(50);
  initMem();

  startMozzi(CONTROL_RATE);
  initMidi(0);
  seq.setNumberOfSteps(32);
  //seq.setTempo(120);
  seq.setGrooveOn();
  hw.update();
  hw.update();
}

void loop(){
  audioHook();
}

void updateControl(){

  MIDI.read();
  hw.update();
  UI();

}














