
#define CONTROL_CHANGE_BITS 7
#define CONTROL_CHANGE_OFFSET 102
unsigned char midiSound;

#define SWITCH_BYTE 14
#define PAGE_BYTE 13

#define BASTL_BYTE 0x7D
#define CHANNEL_BYTE 0x00
#define INSTRUMENT_BYTE 0X01 // poly 1.0
#define TEST_BYTE 0x0A
#define NUMBER_OF_MESSAGE_BYTES 2

#define NUMBER_OF_MESSAGE_BYTES 16
#define NUMBER_OF_SYSEX_BYTES 14
unsigned char sysExArray[NUMBER_OF_SYSEX_BYTES];

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
  
  if(number==SWITCH_BYTE){
    for(int i=0;i<3;i++) hw.setSwitch(i,bitRead(value,i));
  }
  else if(number==PAGE_BYTE){
    page=value;
  }

}

void HandleProgramChange(byte channel, byte number  ){
  // implement preset change
}

void HandlePitchBend(byte channel, int bend){

}

void HandleSystemExclusive(byte *array, byte size){

  if(array[1]==BASTL_BYTE){ 

    if(array[2]==CHANNEL_BYTE){
      inputChannel=array[2]; 
      array[2]++;
      MIDI.sendSysEx(size,array);
    }
    else if(array[2]==INSTRUMENT_BYTE){
      hw.setColor(WHITE);
      hw.update();
      for(int i=0;i<NUMBER_OF_SYSEX_BYTES;i++) sysExArray[i]=array[i+1];
      extractSysExArray(sound);
      hw.freezeAllKnobs();
    }
    else if(array[2]==TEST_BYTE) test=true, MIDI.turnThruOff(), MIDI.sendSysEx(NUMBER_OF_MESSAGE_BYTES,array,false);
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

void indicateMidiChannel(unsigned char _channel){

  boolean highChannel=false;
  if(_channel>=10)   hw.setLed(LED_1,true), hw.setLed(LED_2,true), hw.setLed(LED_3,true), highChannel=true;
  for(int i=0;i<3;i++){
    hw.setLed(_channel-1-highChannel*9,false);
    hw.update();
    delay(150);
    hw.setLed(_channel-1-highChannel*9,true);
    hw.update();
    delay(150);
  }

}


void initMidi(unsigned char _channel){

  MIDI.begin(0);    
  inputChannel=_channel;
  indicateMidiChannel(_channel);
  
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






void sendSysExArray(){
  MIDI.sendSysEx(NUMBER_OF_MESSAGE_BYTES,sysExArray,false);
}


void makeSysExArray(unsigned char _sound){
  unsigned char _bit=0;
  unsigned char _byte=0;
  sysExArray[0]=BASTL_BYTE;
  sysExArray[1]=INSTRUMENT_BYTE;
  for(int i=0;i<NUMBER_OF_VARIABLES;i++){
    int readFrom=getVar(_sound,i);
    for(int j=0;j<variableDepth[i];j++){
      _bit++;
      if(_bit>=7) _bit=0,_byte++;
      bitWrite(sysExArray[_byte+2],_bit, bitRead(readFrom,j));
    }
  } 
}


void extractSysExArray(unsigned char _sound){
  unsigned char _bit=0;
  unsigned char _byte=0;
  for(int i=0;i<NUMBER_OF_VARIABLES;i++){
    int writeTo=0;
    for(int j=0;j<variableDepth[i];j++){
      _bit++;
      if(_bit>=7) _bit=0,_byte++;
      bitWrite(writeTo,j,bitRead(sysExArray[_byte+2],_bit));
    }
    setVar(_sound,i,writeTo);
  } 
}




