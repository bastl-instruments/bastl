
//HUNTER\\

// 3osc - pitch pitch pitch (param wt1 wt2 wt3)
// simple fm - pitch, amt, mod ratio (param wt1 wt2)
// auduino pitch,  phase 1, phase 2(params dec1, dec2)
// simple subtractive  - pitch, cutof, res (param - wT)


// simple am - 3x pitch (param wt1 wt2 wt3)
// simple xor pitch, xor, type / xor osc? (param wt)
// more osc fm - 2xpitch - 1x amt (param
// oneliners ? (param


//  osc 3bit type , 3bit 1param, 2bit 2param

//LFO - amt-2bit, rate- 2bit, dest-2bit, shape-2bit, 
//ADSR 2bit type , atack 3bit,  decay 3bit

//


/*
todo -

fine tune synth, lfo, adsr


tweak single sound - seq
hold - logika
pokemon test
release delay
midi note implementation
midi cc detailed
fine tune osc, lfo, adsr
mystery shuffle

-virtual patch cord

- version for eurorack

*/
//

// libraries
#include <MIDI.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>
#include <trinityHW.h>
#include <MozziGuts.h>
#include <mozzi_rand.h>
#include <mozzi_midi.h>
#include <mozzi_analog.h>

//#include <bastlSample.h>

#include <LowPassFilter.h>
//#include <bastlStateVariable.h>
#include <envelope.h> 
#include <lfo.h>
#include <Oscil.h>
#include <sequencer.h>

#include <bastlTables/haluz2048_int8.h>
#include <tables/sin1024_int8.h>
#include <tables/saw1024_int8.h> 
#include <tables/triangle1024_int8.h> 
//#include <tables/square_analogue512_int8.h> 
 

boolean test;
unsigned char inputChannel;

const char* WAVE_TABLES[]={ //SQUARE_ANALOGUE512_DATA
  SIN1024_DATA, SAW1024_DATA, TRIANGLE1024_DATA, HALUZ2048_DATA};

#define NUMBER_OF_WAVETABLES 4



#define CONTROL_RATE 128 // powers of 2 please

#define NUMBER_OF_OSCILATORS 4
#define METRONOME_OSC 3

Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> aOsc[NUMBER_OF_OSCILATORS]={
  (SIN1024_DATA),(SIN1024_DATA),(SIN1024_DATA),(SIN1024_DATA)};

#define OSC_A 0
#define OSC_B 1
#define OSC_C 2


envelope ADSR;
lfo LFO;
//StateVariable lpf;
LowPassFilter lpf;

//Sample<AUDIO_RATE> aSample[NUMBER_OF_VOICES];
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
//
  startMozzi(CONTROL_RATE);

  //adcReconnectAllDigitalIns();
 
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

  while(MIDI.read(inputChannel));// MIDI.read(inputChannel);
  while(MIDI.read());// MIDI.read();
 
  hw.update();
  UI();
   while(MIDI.read(inputChannel));// MIDI.read(inputChannel);
  while(MIDI.read());
  

}

void loop() {
  audioHook();
}











