
#define CONTROL_CHANGE_BITS 7
#define CONTROL_CHANGE_OFFSET 102
unsigned char midiSound;
void HandleNoteOn(byte channel, byte note, byte velocity) { 
  if(channel==inputChannel){
    if (velocity == 0) {
      HandleNoteOff(channel,note,velocity);
    }
    else{
      playSound(note%6, velocity);
      if(shift && (note%6)>=3) hw.setLed(note%3,true);
      else if(note%6<3) hw.setLed(note%3,true);
      midiSound=note%6;  
    }
  }
}

void HandleNoteOff(byte channel, byte note, byte velocity){
  /*
  unsigned char voice=note%3;
   unsigned char _sound=note%6;
   if(channel==inputChannel){
   if(currentSound[voice]==_sound) ;
   }
   */
}

void HandleControlChange(byte channel, byte number, byte value){
  // implement knob movement
  if(channel==inputChannel){
    if((number-CONTROL_CHANGE_OFFSET )<NUMBER_OF_VARIABLES){
      setVar(midiSound,number-CONTROL_CHANGE_OFFSET,scale(value,CONTROL_CHANGE_BITS,variableDepth[number-CONTROL_CHANGE_OFFSET]));  
      hw.freezeAllKnobs();
      renderTweaking((number-CONTROL_CHANGE_OFFSET)/VARIABLES_PER_PAGE);
    }
  }

}

void HandleProgramChange(byte channel, byte number  ){
  // implement preset change
}

void HandlePitchBend(byte channel, int bend){

}

void HandleSystemExclusive(byte *array, byte size){
  if(array[0]==0x7D){ 
    inputChannel=array[1]; 
    array[1]++;
    MIDI.sendSysEx(size,array);
  }
}

void HandleSongSelect(byte songnumber){

}

void HandleClock(){

}
void HandleStart(){

}
void HandleContinue(){

}
void HandleStop(){

}

void initMidi(unsigned char _channel){

  MIDI.begin(0);    
  inputChannel=_channel;

  MIDI.setHandleNoteOn(HandleNoteOn);
  MIDI.setHandleNoteOff(HandleNoteOff);

  MIDI.setHandlePitchBend(HandlePitchBend);

  MIDI.setHandleControlChange(HandleControlChange);
  MIDI.setHandleProgramChange(HandleProgramChange);
  MIDI.setHandleSystemExclusive(HandleSystemExclusive);
  MIDI.setHandleSongSelect(HandleSongSelect);

  MIDI.setHandleClock(HandleClock);
  MIDI.setHandleStart(HandleStart);
  MIDI.setHandleContinue(HandleContinue);
  MIDI.setHandleStop(HandleStop);

  MIDI.turnThruOn(Full);
  // MIDI.turnThruOff(); 
}






