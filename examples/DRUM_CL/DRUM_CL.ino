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
#include <sequencer.h>
boolean test;
unsigned char inputChannel;

//#include <drumkit_TR.h>
 #include <drumkit_CL.h>
// #include <drumkit_GB.h>
// #include <drumkit_GL.h>



#define NUMBER_OF_VOICES 6

#define CONTROL_RATE 128 // powers of 2 please

Sample<AUDIO_RATE> aSample[NUMBER_OF_VOICES];
//Sample<AUDIO_RATE> aMetro;

sequencer seq(61);
trinityHW hw; // MOZZI or DEFAULT
//boolean bootShift=false;
#define DEFAULT_KNOB_TOLERANCE 15

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
  for(int i=0;i<NUMBER_OF_VOICES;i++) aSample[i].setTable(WAVE_TABLES[i],WAVE_TABLES_NUM_CELLS[i]),  aSample[i].setTimeStretch(5); //aSample[i].setTimeStretchOn(),
  aSample[5].setTable(WAVE_TABLES[1],WAVE_TABLES_NUM_CELLS[1]);
  hw.update();
  hw.update();
  //bootShift=hw.buttonState(SMALL_BUTTON_1);
  seq.setNumberOfSteps(32);
  //seq.setTempo(120);
  seq.setGrooveOn();
  // aMetro.setTable(SNARE2_DATA,SNARE2_NUM_CELLS);
  //aSample[5].setTable(GB_SNARE_DATA,GB_SNARE_NUM_CELLS);
  // aSample[5].setTable(SNARE2_DATA,SNARE2_NUM_CELLS);
  hw.freezeAllKnobs();
  loadPreset(0);
  hw.setKnobTolerance(DEFAULT_KNOB_TOLERANCE);
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







