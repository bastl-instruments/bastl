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


/*
// START
 #include <bastlSamples/TR_KICK.h>
 #include <bastlSamples/TR_SNARE.h> 
 #include <bastlSamples/TR_HH.h> 
 #include <bastlSamples/TR_CB.h> 
 #include <bastlSamples/TR_TOM.h> 
 #include <bastlSamples/TR_CLAP.h> 
 
 #include <bastlTables/haluz2048_int8.h> 
 
 const char* WAVE_TABLES[]={
 TR_KICK_DATA, TR_SNARE_DATA, TR_HH_DATA,   TR_CB_DATA, TR_CLAP_DATA , TR_TOM_DATA}; //TR_OH_DATA, // TR_RIM_DATA,
 
 const unsigned int WAVE_TABLES_NUM_CELLS[]={
 TR_KICK_NUM_CELLS, TR_SNARE_NUM_CELLS, TR_HH_NUM_CELLS,   TR_CB_NUM_CELLS, TR_CLAP_NUM_CELLS , TR_TOM_NUM_CELLS};//TR_OH_NUM_CELLS, //, TR_TOM_NUM_CELLS TR_RIM_NUM_CELLS,
 #define NUMBER_OF_WAVETABLES 6
 //END
 */
/*
// START
 #include <bastlSamples/KICK2.h>
 #include <bastlSamples/SNARE2.h> 
 #include <bastlSamples/HAT2.h> 
 #include <bastlSamples/CB4.h> 
 #include <bastlTables/haluz2048_int8.h> 
 
 const char* WAVE_TABLES[]={
 KICK2_DATA, SNARE2_DATA, HAT2_DATA,CB4_DATA,HALUZ2048_DATA};
 
 const unsigned int WAVE_TABLES_NUM_CELLS[]={
 KICK2_NUM_CELLS, SNARE2_NUM_CELLS, HAT2_NUM_CELLS, CB4_NUM_CELLS,HALUZ2048_NUM_CELLS};
 #define NUMBER_OF_WAVETABLES 6
 //END
 
 
 */
 
// START
#include <bastlSamples/GB_KICK.h>
#include <bastlSamples/GB_SNARE.h> 
#include <bastlSamples/GB_HH.h> 
#include <bastlSamples/GB_TUI.h> 
#include <bastlTables/haluz2048_int8.h> 
//#include <bastlSamples/GB_BLIP.h> 
//#include <bastlSamples/GB_BZZ.h> 

const char* WAVE_TABLES[]={
  GB_KICK_DATA, GB_SNARE_DATA, GB_HH_DATA,GB_TUI_DATA,HALUZ2048_DATA};//,GB_BZZ_DATA};

const unsigned int WAVE_TABLES_NUM_CELLS[]={
  GB_KICK_NUM_CELLS, GB_SNARE_NUM_CELLS, GB_HH_NUM_CELLS,GB_TUI_NUM_CELLS,HALUZ2048_NUM_CELLS};//,GB_BZZ_NUM_CELLS};
#define NUMBER_OF_WAVETABLES 5
//END

/*
// START
#include <bastlSamples/GL_A.h>
#include <bastlSamples/GL_B.h>
#include <bastlSamples/GL_C.h>
#include <bastlSamples/GL_D.h>
#include <bastlSamples/GL_E.h>
#include <bastlSamples/GL_F.h>
#include <bastlSamples/GL_G.h>
#include <bastlSamples/GL_H.h>
//#include <bastlSamples/GL_I.h>
 
#include <bastlTables/haluz2048_int8.h> 
//#include <bastlSamples/GB_BLIP.h> 
//#include <bastlSamples/GB_BZZ.h> 

const char* WAVE_TABLES[]={
  GL_A_DATA, GL_B_DATA, GL_C_DATA, GL_D_DATA, GL_E_DATA, GL_F_DATA, GL_G_DATA, GL_H_DATA};//, GL_I_DATA, };//,GB_BZZ_DATA};

const unsigned int WAVE_TABLES_NUM_CELLS[]={
  GL_A_NUM_CELLS, GL_B_NUM_CELLS, GL_C_NUM_CELLS, GL_D_NUM_CELLS, GL_E_NUM_CELLS, GL_F_NUM_CELLS, GL_G_NUM_CELLS, GL_H_NUM_CELLS};//, GL_I_NUM_CELLS, };//,GB_BZZ_NUM_CELLS};
#define NUMBER_OF_WAVETABLES 8
//END
*/



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







