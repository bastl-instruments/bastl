// http://arduinomidilib.sourceforge.net/class_m_i_d_i___class.html
unsigned char inputChannel;
#define NUMBER_OF_MESSAGE_BYTES 6
#define NUMBER_OF_SYSEX_BYTES 4
unsigned char sysExArray[NUMBER_OF_SYSEX_BYTES];

#define SWITCH_BYTE 14
#define PAGE_BYTE 13

#define BASTL_BYTE 0x7D
#define CHANNEL_BYTE 0x00
#define INSTRUMENT_BYTE 0X01 // poly 1.0
#define TEST_BYTE 0x0A


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
 if(number==SWITCH_BYTE){
    for(int i=ZERO;i<3;i++) hw.setSwitch(i,bitRead(value,i));
  }
  else if(number==PAGE_BYTE){
    page=value;
  }
}

void HandleProgramChange(byte channel, byte number  ){

}
void HandlePitchBend(byte channel, int bend){

}



void HandleSystemExclusive(byte *array, byte size){

  if(array[1]==BASTL_BYTE){ 
   if(array[2]==TEST_BYTE) test=true, MIDI.turnThruOff(), MIDI.sendSysEx(NUMBER_OF_MESSAGE_BYTES,array,false);
  }

}


void HandleSongSelect(byte songnumber){

}

void HandleClock(){
  seq.clockIn();
  turnToSlave();

}
void HandleStart(){
  seq.play();
  turnToSlave();
}
void HandleContinue(){
  seq.midiContinue();

}
void HandleStop(){
  seq.stop();
  sendAllNoteOff();  
}

void turnToSlave(){
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














