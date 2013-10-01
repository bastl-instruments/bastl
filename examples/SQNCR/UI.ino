boolean record, erease;
boolean slave;
boolean combo;
boolean showType;
boolean shift;
boolean jump;
unsigned char jumpStep;
unsigned char page;
#define NUMBER_OF_PAGES 3
boolean lastClockCount;
unsigned char currentPattern[NUMBER_OF_INSTRUMENTS];
unsigned char midiNoteOn[NUMBER_OF_INSTRUMENTS][6];
unsigned char lastStep[NUMBER_OF_INSTRUMENTS];
unsigned char outChannel[NUMBER_OF_INSTRUMENTS]={
  1,2,3};

#define POLYPHONIC 0 
#define POLYPHONIC_LEGATO 1 
#define MONOPHONIC 2 

#define NUMBER_OF_SOUNDS 6 

#define DEFAULT_VELOCITY 127 
#define OFF_VELOCITY 0

void UI(){

  // hw.setColor(page+1);

  renderSmallButtons();
  renderCombo();
  if(!combo) renderBigButtons();
  if(lastClockCount!=seq.clockCount()) recordFromMidi();
  lastClockCount=seq.clockCount();
  renderKnobs();
  renderSequencer();
  if(!seq.isPlaying()) showLeds();




}
void recordFromMidi(){
  for(int j=0;j<3;j++){
    switch(instrumentType[j]){

    case MONOPHONIC:
      /*
      for(int i=0;i<3;i++){
       if(midiNoteOn[j][i]){
       hw.setSwitch(i,midiNoteOn[j][i]);
       if(seq.clockCount()>3){
       if(seq.getCurrentStep()==(seq.getNumberOfSteps()-1)) writeToStep(j,currentPattern[j],0,i,hw.switchState(i));
       else writeToStep(j,currentPattern[j],seq.getCurrentStep()+1,i,hw.switchState(i));
       }
       else writeToStep(j,currentPattern[j],seq.getCurrentStep(),i,hw.switchState(i));)
       midiNoteOn[j][i]=false;
       }
       }
       */
      break;

    case POLYPHONIC:
      for(int i=0;i<6;i++){
        if(midiNoteOn[j][i]){
          if(seq.clockCount()>3){
            if(seq.getCurrentStep()==(seq.getNumberOfSteps()-1)) writeToStep(j,currentPattern[j],0,i,true);
            else writeToStep(j,currentPattern[j],seq.getCurrentStep()+1,i,true);
          }
          else writeToStep(j,currentPattern[j],seq.getCurrentStep(),i,true);
          midiNoteOn[j][i]=false;
        }
      }


      break;

    case POLYPHONIC_LEGATO:

      for(int i=0;i<6;i++){
        if(midiNoteOn[j][i]){
          if(seq.clockCount()>3){
            if(seq.getCurrentStep()==(seq.getNumberOfSteps()-1)) writeToStep(j,currentPattern[j],0,i,true);
            else writeToStep(j,currentPattern[j],seq.getCurrentStep()+1,i,true);
          }
          else writeToStep(j,currentPattern[j],seq.getCurrentStep(),i,true);

        }
      }

      break;

    }
  }
}
void renderSequencer(){

  seq.update(audioTicks());
  if(!slave) {
    while(seq.clockReady()) seq.clockIn(), sendClock() ;
  } 
  while(seq.stepReady()) {
    seq.stepIn();
    if(jump) seq.jumpToStep(jumpStep);
    proceedStep(seq.getCurrentStep()); 
  }

}


void showLeds(){

  if(showType){
    hw.setColor(instrumentType[page]+1);
  }
  else hw.setColor(page+1);

}

void renderSmallButtons(){
  shift=hw.buttonState(SMALL_BUTTON_1);
  if(hw.justPressed(SMALL_BUTTON_1) || hw.justReleased(SMALL_BUTTON_1)) hw.freezeAllKnobs();

  erease=hw.buttonState(EXTRA_BUTTON_2);
  //if(hw.justPressed(SMALL_BUTTON_1)) seq.tapIn(audioTicks()); 
  if(hw.justReleased(SMALL_BUTTON_2) && !combo) page=increaseValue(page,NUMBER_OF_PAGES-1);
}


void renderBigButtons(){
  for(int i=0;i<3;i++){
    unsigned char _step=0;
    //instrument type

    //RECORD
    if(record){
      //EREASE
      if(lastClockCount!=seq.clockCount()){
        if(erease){
          if(hw.buttonState(i)){


            if(seq.clockCount()>3){
              if(seq.getCurrentStep()==(seq.getNumberOfSteps()-1)) writeToStep(page,currentPattern[page],0,shift*3+i,false);
              else writeToStep(page,currentPattern[page],seq.getCurrentStep()+1,shift*3+i,false);
            }
            else writeToStep(page,currentPattern[page],seq.getCurrentStep(),shift*3+i,false);
          } 
        }
      }
      //EREASE


      else{
        switch(instrumentType[page]){

        case MONOPHONIC:

          if(hw.justPressed(i)) hw.flipSwitch(i);

          if(lastClockCount!=seq.clockCount()){
            if(seq.clockCount()>3){
              if(seq.getCurrentStep()==(seq.getNumberOfSteps()-1)) writeToStep(page,currentPattern[page],0,i,hw.switchState(i));
              else writeToStep(page,currentPattern[page],seq.getCurrentStep()+1,i,hw.switchState(i));
            }
            else writeToStep(page,currentPattern[page],seq.getCurrentStep(),i,hw.switchState(i));
          }

          break;

        case POLYPHONIC:

          if(hw.justPressed(i)){
            if(seq.clockCount()>3){
              if(seq.getCurrentStep()==(seq.getNumberOfSteps()-1)) writeToStep(page,currentPattern[page],0,shift*3+i,true);
              else writeToStep(page,currentPattern[page],seq.getCurrentStep()+1,shift*3+i,true);
            }
            else writeToStep(page,currentPattern[page],seq.getCurrentStep(),shift*3+i,true), MIDI.sendNoteOn(3*shift+i,DEFAULT_VELOCITY,outChannel[page]);
          }

          break;

        case POLYPHONIC_LEGATO:
          if(lastClockCount!=seq.clockCount()){
            if(hw.buttonState(i)){

              if(seq.clockCount()>3){
                if(seq.getCurrentStep()==(seq.getNumberOfSteps()-1)) writeToStep(page,currentPattern[page],0,shift*3+i,true);
                else writeToStep(page,currentPattern[page],seq.getCurrentStep()+1,shift*3+i,true);
              }
              else writeToStep(page,currentPattern[page],seq.getCurrentStep(),shift*3+i,true), MIDI.sendNoteOn(3*shift+i,DEFAULT_VELOCITY,outChannel[page]);
            }
          }

          break;

        }  

      }
    }

    //RECORD


    //PLAYBACK
    else{
      switch(instrumentType[page]){

      case MONOPHONIC:
        if(!seq.isPlaying()){
          if(hw.justPressed(i)){
            MIDI.sendNoteOff(hw.soundFromSwitches(),OFF_VELOCITY,outChannel[page]);
            hw.flipSwitch(i);
            MIDI.sendNoteOn(hw.soundFromSwitches(),DEFAULT_VELOCITY,outChannel[page]);
          }
          hw.setLed(i,hw.switchState(i));
        }
        break;

      case POLYPHONIC:
        if(hw.justPressed(i)) MIDI.sendNoteOn(3*shift+i,DEFAULT_VELOCITY,outChannel[page]);
        if(hw.justReleased(i)) MIDI.sendNoteOff(3*shift+i,OFF_VELOCITY,outChannel[page]);
        hw.setLed(i,hw.buttonState(i));
        break;
      case POLYPHONIC_LEGATO:
        if(hw.justPressed(i)) MIDI.sendNoteOn(3*shift+i,DEFAULT_VELOCITY,outChannel[page]);
        if(hw.justReleased(i)) MIDI.sendNoteOff(3*shift+i,OFF_VELOCITY,outChannel[page]);
        if(!seq.isPlaying()) hw.setLed(i,hw.buttonState(i));
        break;
      }

    }
    //PLAYBACK

  }
}


void renderCombo(){
  // if(hw.buttonState(SMALL_BUTTON_2) && hw.justPressed(SMALL_BUTTON_1)) loadPreset(0), hw.freezeAllKnobs(), combo=true;
  for(int i=0;i<3;i++){
    //  if(hw.buttonState(EXTRA_BUTTON_2) && hw.justPressed(i)) randomize(i+3*shift); // randomize 
    if(hw.buttonState(SMALL_BUTTON_1) && hw.buttonState(SMALL_BUTTON_2) && hw.justPressed(i)) storePreset(currentPreset), loadPreset(i), hw.freezeAllKnobs(), combo=true; // save&load
    if(hw.buttonState(SMALL_BUTTON_2) && hw.justPressed(i)) loadPreset(i), hw.freezeAllKnobs(), combo=true; // load

  }

  for(int i=0;i<3;i++){
    if(hw.buttonState(EXTRA_BUTTON_1) && hw.buttonState(i)){
      jump=true;
      jumpStep=(i*4+shift*16);
      combo=true;
      break; 
    }
    else jump=false; //&& lastClockCount!=seq.clockCount() && seq.clockCount()<3
  }

  if(hw.buttonState(EXTRA_BUTTON_1) && hw.justPressed(EXTRA_BUTTON_2)) seq.tapIn(audioTicks()), combo=true;

  if(hw.buttonState(SMALL_BUTTON_2) && hw.justPressed(EXTRA_BUTTON_2)){
    seq.playStop(), combo=true; 
    if(seq.isPlaying() && !slave) MIDI.sendRealTime(Start);
    else if(!slave) MIDI.sendRealTime(Stop), sendAllNoteOff();
  }

  if(hw.buttonState(EXTRA_BUTTON_2) && hw.justPressed(SMALL_BUTTON_2))  record=!record, combo=true; // seq.start();
  if(!seq.isPlaying()) record=false;

  if(hw.buttonState(SMALL_BUTTON_1) && hw.buttonState(EXTRA_BUTTON_1) && hw.justPressed(SMALL_BUTTON_2)) instrumentType[page]=increaseValue(instrumentType[page],2), combo=true;
  if(hw.buttonState(SMALL_BUTTON_1) && hw.buttonState(EXTRA_BUTTON_1) && hw.justPressed(EXTRA_BUTTON_2)) clearPattern(page,currentPattern[page]); 
  combo=true;

  if(hw.buttonState(SMALL_BUTTON_1) && hw.buttonState(EXTRA_BUTTON_1)) showType=true;
  else showType=false;

  if(combo){
    //turn off combo when all buttons are released 
    unsigned char _count=0; 
    for(int i=0;i<NUMBER_OF_BUTTONS;i++)  _count+=hw.buttonState(i); // if(!hw.buttonState(i)) combo=false;
    if(_count==0) combo=false;
    //else combo=true;
  } 

}



void renderKnobs(){


  if(shift){

    if(hw.knobFreezed(KNOB_TOP)) {
      if(inBetween(20+hw.knobValue(KNOB_TOP)>>2,20+hw.lastKnobValue(KNOB_TOP)>>2,seq.getTempo()  )) hw.unfreezeKnob(KNOB_TOP); //external unfreez
    }
    else seq.setTempo(20+hw.knobValue(KNOB_TOP)>>2);    

    if(hw.knobFreezed(KNOB_LEFT)) {
      if(inBetween(1+hw.knobValue(KNOB_LEFT)>>5,1+hw.lastKnobValue(KNOB_LEFT)>>5,seq.getNumberOfSteps()  )) hw.unfreezeKnob(KNOB_LEFT); //external unfreez
    }
    else seq.setNumberOfSteps(1+hw.knobValue(KNOB_LEFT)>>5);   

    if(hw.knobFreezed(KNOB_RIGHT)) {
      if(inBetween(hw.knobValue(KNOB_RIGHT)>>2,hw.lastKnobValue(KNOB_RIGHT)>>2,seq.getGrooveAmount()  )) hw.unfreezeKnob(KNOB_RIGHT); //external unfreez
    }
    else seq.setGrooveAmount(hw.knobValue(KNOB_RIGHT)>>2); 



  }
  else{
    if(hw.knobFreezed(KNOB_TOP)) {
      if(inBetween( hw.knobValue(KNOB_TOP)>>8,hw.lastKnobValue(KNOB_TOP)>>8,currentPattern[1]  )) hw.unfreezeKnob(KNOB_TOP); //external unfreez
    }
    else currentPattern[1]=hw.knobValue(KNOB_TOP)>>8;    

    if(hw.knobFreezed(KNOB_LEFT)) {
      if(inBetween( hw.knobValue(KNOB_LEFT)>>8,hw.lastKnobValue(KNOB_LEFT)>>8,currentPattern[0]  )) hw.unfreezeKnob(KNOB_LEFT); //external unfreez
    }
    else currentPattern[0]=hw.knobValue(KNOB_LEFT)>>8;   

    if(hw.knobFreezed(KNOB_RIGHT)) {
      if(inBetween( hw.knobValue(KNOB_RIGHT)>>8,hw.lastKnobValue(KNOB_RIGHT)>>8,currentPattern[2]  )) hw.unfreezeKnob(KNOB_RIGHT); //external unfreez
    }
    else currentPattern[2]=hw.knobValue(KNOB_RIGHT)>>8;      
  }

}



void proceedStep(unsigned char _step){

  showTempo(_step);
  renderMetronome(_step);

  for(int instrument;instrument<NUMBER_OF_INSTRUMENTS;instrument++){
    unsigned char _STEP=getStep(instrument,currentPattern[instrument],_step);
    interpretStep(instrument,_STEP);
    if(instrument==page) showStep(instrument,_STEP);
  }

}

void showTempo(unsigned char _step){

  if(showType){
    hw.setColor(instrumentType[page]+1);
  }
  else{

    if(record){
      if(_step%4==0) hw.setLed(LED_R+page,false);
      else hw.setColor(WHITE);
      if(_step==0) hw.setColor(BLACK); 
    }
    else{
      if(_step%4==0) hw.setColor(page+1) ;
      else hw.setColor(BLACK);
      if(_step==0) hw.setColor(WHITE); 
    }
  }

}



void renderMetronome(unsigned char _step){

  if(_step%4==0) aSample.setFreq(50), aSample.start();
  if(_step%16==0) aSample.setFreq(100); 

}
void showStep(unsigned char instrument,unsigned char _step){

  for(int i=0;i<NUMBER_OF_SOUNDS;i++){
    if(shift) {
      if(i>=3) hw.setLed(i-3,bitRead(_step,i));
    }
    else{
      if(i<3) hw.setLed(i,bitRead(_step,i));
    }
  }


}



void interpretStep(unsigned char instrument,unsigned char _step){

  switch(instrumentType[instrument]){
  case MONOPHONIC:
    {

      unsigned char _send;
      for(int i=0;i<3;i++){
        bitWrite(_send,i,bitRead(_step,i));
      }
      if(lastStep[instrument]!=_send){
        MIDI.sendNoteOff(lastStep[instrument],OFF_VELOCITY,outChannel[instrument]);
        MIDI.sendNoteOn(_send,DEFAULT_VELOCITY,outChannel[instrument]);
      }
      lastStep[instrument]=_send;
      break;
    }
  case POLYPHONIC:

    for(int i=0;i<NUMBER_OF_SOUNDS;i++){
      if(bitRead(_step,i)) MIDI.sendNoteOff(i,OFF_VELOCITY,outChannel[instrument]), MIDI.sendNoteOn(i,DEFAULT_VELOCITY,outChannel[instrument]);
    }
    lastStep[instrument]=_step;
    break;
  case POLYPHONIC_LEGATO:

    for(int i=0;i<NUMBER_OF_SOUNDS;i++){
      if( !bitRead(_step,i) && bitRead(lastStep[instrument],i)) MIDI.sendNoteOff(i,OFF_VELOCITY,outChannel[instrument]);
      if(bitRead(_step,i) && !bitRead(lastStep[instrument],i)) MIDI.sendNoteOn(i,DEFAULT_VELOCITY,outChannel[instrument]);
    }

    lastStep[instrument]=_step;
    break;
  } 

}


// UTILITIES

int increaseValue(int _VALUE, int _OVERFLOW){
  if(_VALUE < _OVERFLOW) _VALUE++;
  else _VALUE=0;
  return _VALUE;
}

int increaseValue(int _VALUE, int _OVERFLOW, int _INCREMENT){
  if(_VALUE < _OVERFLOW) _VALUE+=_INCREMENT;
  return _VALUE;
}

int decreaseValue(int _VALUE, int _OVERFLOW, int _INCREMENT){
  if(_VALUE > _OVERFLOW) _VALUE-=_INCREMENT;
  return _VALUE;
}


unsigned char getBits(unsigned char _val, unsigned char _offset, unsigned char _bits){
  unsigned char returnVal=0;
  for(int i=0;i<_bits;i++){
    bitWrite(returnVal,i,bitRead(_val,_offset+i));
  }
  return  returnVal;
}


unsigned char writeBits(unsigned char _val, unsigned char _offset, unsigned char _bits, unsigned char _toWrite){

  for(int i=0;i<_bits;i++){
    bitWrite(_val,_offset+i,bitRead(_toWrite,i));
  }
  return _val;
}


void writeToStep(unsigned char _instrument,unsigned char _pattern, unsigned char _step, unsigned char _bit, boolean _state){
  unsigned char _what=0;
  _what=getStep(_instrument,_pattern,_step); 
  bitWrite(_what,_bit,_state) ;
  setStep(_instrument,_pattern,_step,_what);
}


void sendClock(){
  MIDI.sendRealTime(Clock);
}


void clearPattern(unsigned char _page,unsigned char _pattern){
  for(int i=0;i<32;i++){
    for(int j=0;j<6;j++){
      writeToStep(_page,_pattern,i,j,false);
    }
  }
}


void sendAllNoteOff(){
  for(int i=0;i<NUMBER_OF_INSTRUMENTS;i++){
    switch(instrumentType[i]){

    case MONOPHONIC:
      MIDI.sendNoteOn(hw.soundFromSwitches(),DEFAULT_VELOCITY,outChannel[i]);
      break;
    default:
      for(int j=0;j<6;j++){
        MIDI.sendNoteOff(j,OFF_VELOCITY,outChannel[i]);
      }
      break;
    } 
  }
}























































