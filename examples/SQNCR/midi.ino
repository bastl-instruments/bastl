// http://arduinomidilib.sourceforge.net/class_m_i_d_i___class.html
unsigned char inputChannel;



void HandleNoteOn(byte channel, byte note, byte velocity) { 

  if (velocity == 0) {
    HandleNoteOff(channel,note,velocity);
  }
  else{

    if(record){
      if(channel<=NUMBER_OF_INSTRUMENTS){
        switch(instrumentType[channel-1]){

        case MONOPHONIC:
         // for(int i=0;i<3;i++) midiNoteOn[channel-1][i]=bitRead(note%8,i);
          
          for(int i=0;i<3;i++) hw.setSwitch(i,bitRead(note%8,i));

          break;

        case POLYPHONIC:
          midiNoteOn[channel-1][note%6]=true;

          break; 

        case POLYPHONIC_LEGATO:
          if(note<6) midiNoteOn[channel-1][note]=true;
          break; 
        }
      }

    }

  }

}

void HandleNoteOff(byte channel, byte note, byte velocity){


  if(record){
    if(channel<=NUMBER_OF_INSTRUMENTS){
      switch(instrumentType[channel-1]){

      case MONOPHONIC:

        break;

      case POLYPHONIC:

        break; 

      case POLYPHONIC_LEGATO:
        if(note<6) midiNoteOn[channel-1][note]=false;
        break; 
      }
    }

  }
}

void HandleControlChange(byte channel, byte number, byte value){

}

void HandleProgramChange(byte channel, byte number  ){

}
void HandlePitchBend(byte channel, int bend){

}
void HandleSystemExclusive(byte *array, byte size){

}

void HandleSongSelect(byte songnumber){

}

void HandleClock(){
  seq.clockIn();
  slave=true;
}
void HandleStart(){
  seq.play();
  slave=true;
}
void HandleContinue(){
  seq.midiContinue();
  slave=true;
}
void HandleStop(){
  seq.stop();
  sendAllNoteOff();  
  slave=true;
}

void initMidi(unsigned char _channel){

  MIDI.begin(_channel);    
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













