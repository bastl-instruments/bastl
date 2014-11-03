/*

example code running Mozzi library on Trinity synth by Bastl Instruments
including bastl library

-3 synthesis types - knobs adjust oscilators and parameters
-big buttons change wavetables of oscilators
-left small button changes between tuned oscilators and free running oscilators
-right small button changes synthesis types (colors)

example written by Vaclav Pelousek
www.bastl-instruments.com

*/
#include <trinityHW.h>
#include <MozziGuts.h>
#include <mozzi_midi.h>
#include <Oscil.h>
#include <mozzi_analog.h>

// tables / samples
#include <tables/sin2048_int8.h> // 
#include <tables/saw2048_int8.h> // 
#include <tables/triangle2048_int8.h> // 

//array of names of wavetables
const int8_t* WAVE_TABLES[]={
  SIN2048_DATA, SAW2048_DATA,TRIANGLE2048_DATA};

#define NUMBER_OF_OSCILATORS 3
#define NUMBER_OF_WAVETABLES 3

//create oscilators
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aOsc[NUMBER_OF_OSCILATORS]={
  (SIN2048_DATA),(SIN2048_DATA),(SIN2048_DATA)};

//constructor for hardware layer
trinityHW hw;

#define CONTROL_RATE 64

void setup() {
  //initiate libraries
  hw.initialize(DEFAULT);
  startMozzi(CONTROL_RATE);

}

void loop() {
  audioHook(); // required here
}


//same variables
int table[3];
#define NUMBER_OF_PAGES 3
#define SIMPLE_SINE 0
#define FM_SYNTH 1
#define AM_SYNTH 2

unsigned char page;
unsigned char modAmt;
boolean tuned=false;

//this runs at control rate frequency - interupt driven
void updateControl(){

  hw.update(); // update inputs from interface
  UI(); // proceed user interface

}

void UI(){

  // switch between tuned oscilators and free frequency
  if(hw.justPressed(SMALL_BUTTON_1)) tuned=!tuned;

  // switch different pages of settings
  if(hw.justPressed(SMALL_BUTTON_2)) page=increaseValue(page,NUMBER_OF_PAGES-1);
  hw.setColor(page+1);

  // on button press change wavetable according to specific knob
  if(hw.justPressed(BIG_BUTTON_1)) table[LEFT_KNOB]=increaseValue(table[LEFT_KNOB],NUMBER_OF_WAVETABLES-1), aOsc[LEFT_KNOB].setTable(WAVE_TABLES[table[LEFT_KNOB]]);
  if(hw.justPressed(BIG_BUTTON_2)) table[TOP_KNOB]=increaseValue(table[TOP_KNOB],NUMBER_OF_WAVETABLES-1), aOsc[TOP_KNOB].setTable(WAVE_TABLES[table[TOP_KNOB]]);
  if(hw.justPressed(BIG_BUTTON_3)) table[RIGHT_KNOB]=increaseValue(table[RIGHT_KNOB],NUMBER_OF_WAVETABLES-1), aOsc[RIGHT_KNOB].setTable(WAVE_TABLES[table[RIGHT_KNOB]]);

  //according to different page proceed different synthesis type
  switch(page){

  case SIMPLE_SINE:
  
    if(tuned){
      for(int i=0;i<3;i++) aOsc[i].setFreq(hw.knobValue(i));
    }
    else{
      for(int i=0;i<3;i++) aOsc[i].setFreq(mtof(24+(hw.knobValue(i)>>4)));
    }
    
    break;

  case AM_SYNTH:
  
    if(tuned){
      for(int i=0;i<3;i++) aOsc[i].setFreq(hw.knobValue(i));
    }
    else{
      for(int i=0;i<3;i++) aOsc[i].setFreq(mtof(24+(hw.knobValue(i)>>4)));
    }
    
    break;

  case FM_SYNTH:

    int carrierFreq;

    if(tuned) carrierFreq=mtof(24+(hw.knobValue(TOP_KNOB)>>4));
    else carrierFreq=hw.knobValue(TOP_KNOB);

    aOsc[0].setFreq(carrierFreq);
    float ratio= (float) hw.knobValue(LEFT_KNOB)/64;
    float modFreq=(float)carrierFreq*ratio;
    aOsc[1].setFreq(modFreq);
    modAmt=hw.knobValue(RIGHT_KNOB)>>2;

    break;

  } 

}


int updateAudio(){

  unsigned char shiftRight=0;
  int value=0;
  
  // render sound output according to the synthesis type
  switch(page){ 

  case SIMPLE_SINE:
    for(int i=0;i<NUMBER_OF_OSCILATORS;i++) value+=aOsc[i].next();
    shiftRight=4;
    break;
  case FM_SYNTH:
    value=aOsc[0].phMod(modAmt*aOsc[1].next());
    shiftRight=5;
    break;
  case AM_SYNTH:
    value=((aOsc[2].next()*aOsc[1].next()>>8)*aOsc[0].next())>>2;
    shiftRight=0;
    break;
  }
  return value<<shiftRight; // this goes to the output

}


// just an utility
int increaseValue(int _VALUE, int _OVERFLOW){

  if(_VALUE < _OVERFLOW) _VALUE++;
  else _VALUE=0;
  return _VALUE;

}




