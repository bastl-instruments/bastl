/*
 written by Vaclav Pelousek
 www.bastl-instruments.com
 */

#include <MIDI.h>
#include <trinityHW.h>
#include <MozziGuts.h>
#include <mozzi_midi.h>
#include <Oscil.h>
#include <mozzi_analog.h>

// tables / samples
#include <tables/sin2048_int8.h> // 



#define NUMBER_OF_OSCILATORS 3
#define NUMBER_OF_WAVETABLES 3

//create oscilators
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aOsc[NUMBER_OF_OSCILATORS]={
  (SIN2048_DATA),(SIN2048_DATA),(SIN2048_DATA)};

//constructor for hardware layer
trinityHW hw;

#define CONTROL_RATE 64

boolean testing;
int counter;
#define COUNTER_LIMIT 32
#define DEFAULT_CHANNEL 1
#define DEFAULT_VELOCITY 127
boolean flip;


void setup() {
  //initiate libraries
  hw.initialize(DEFAULT);
  startMozzi(CONTROL_RATE);
  initMidi(0);

}

void loop() {
  audioHook(); // required here
}


void updateControl(){

  MIDI.read();
  MIDI.read();
  MIDI.read();
  MIDI.read();
  MIDI.read();
  MIDI.read();
  MIDI.read();
  MIDI.read();
  hw.update(); // update inputs from interface
  UI(); // proceed user interface

}


unsigned char page;
boolean sendSwitch;
#define SWITCH_BYTE 14
#define PAGE_BYTE 13

void UI(){


  if(testing){
    if(hw.justPressed(SMALL_BUTTON_1)) page=increaseValue(page,2) , MIDI.sendControlChange(PAGE_BYTE,page,1);
    if(hw.justPressed(SMALL_BUTTON_2)) page=decreaseValue(page,2), MIDI.sendControlChange(PAGE_BYTE,page,1);
    hw.setColor(page+1);

    for(int i=0;i<3;i++){
      if(hw.justPressed(i))  hw.flipSwitch(i), sendSwitch=true;
      hw.setLed(i,hw.switchState(i));
    //  hw.unfreezeKnob(i);
    //  if(hw.knobMoved(i)) MIDI.sendControlChange(10+i,hw.knobValue(i)>>3,1),  hw.setColor(WHITE);
    }
    if(sendSwitch) MIDI.sendControlChange(SWITCH_BYTE,hw.soundFromSwitches(),1), sendSwitch=false;
  }
  else{
    counter++;
    if(counter>32) counter=0,page=increaseValue(page,7),sendSysExArray();
    hw.setColor(page);
  }

}

#define NUMBER_OF_MESSAGE_BYTES 2
#define BASTL_BYTE 0x7D
#define CHANNEL_BYTE 0x00
#define INSTRUMENT_BYTE 0X01 // poly 1.0
#define TEST_BYTE 0x0A
unsigned char sysExArray[NUMBER_OF_MESSAGE_BYTES]={
  BASTL_BYTE,TEST_BYTE};


void sendSysExArray(){

  MIDI.sendSysEx(NUMBER_OF_MESSAGE_BYTES,sysExArray,false);

}

int increaseValue(int _VALUE, int _OVERFLOW){
  if(_VALUE < _OVERFLOW) _VALUE++;
  else _VALUE=0;
  return _VALUE;
}

int decreaseValue(int _VALUE, int _OVERFLOW){
  if(_VALUE > 0) _VALUE--;
  else _VALUE=_OVERFLOW;
  return _VALUE;
}



int updateAudio(){

  int value=0;

  for(int i=0;i<NUMBER_OF_OSCILATORS;i++) value+=aOsc[i].next();

  return value<<4; 

}



