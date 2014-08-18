

#define CONTROL_CHANGE_BITS 7
#define CONTROL_CHANGE_OFFSET 102
#define ARP_VELOCITY 120
#define SWITCH_BYTE 14
#define PAGE_BYTE 13

unsigned char midiSound;

#define POLYPHONY NUMBER_OF_VOICES
boolean thereIsNoteToPlay;

#define BUFFER_SIZE 16
unsigned char midiBuffer[BUFFER_SIZE];
unsigned char voiceUse[NUMBER_OF_VOICES]={
  255,255,255,255,255,255};

unsigned char fromBuffer;
boolean ping;


void shiftBufferLeft(unsigned char from){
  for(int i=from;i<notesInBuffer;i++){
    midiBuffer[i]=midiBuffer[i+1]; 
  }
}

void shiftBufferRight(){
  for(int i=notesInBuffer;i>0;i--){
    midiBuffer[i]=midiBuffer[i-1]; 
  }
}

boolean isThereNoteToPlay(){
  return thereIsNoteToPlay;

}
unsigned char noteToPlay(){
  thereIsNoteToPlay=false;
  return midiBuffer[fromBuffer];
}

unsigned char freeVoice(unsigned char note){

  unsigned char use=255;
  for(int i=0;i<NUMBER_OF_VOICES;i++){
    if(voiceUse[i]==note){
      voiceUse[i]=255, use=i;
      break;
    }
  }
  return use;

}

boolean pink;
unsigned char getFreeVoice(unsigned char note){

  unsigned char use=255;
  /*
  for(int i=0;i<NUMBER_OF_VOICES;i++){
   if(voiceUse[i]==255) {
   voiceUse[i]=note, use=i;
   break; 
   }
   }
   if(use<NUMBER_OF_VOICES) return use;
   
   */
  pink=!pink;
  if(pink){

    for(int i=0;i<NUMBER_OF_VOICES;i++){
      if(voiceUse[i]==255) {
        voiceUse[i]=note, use=i;
        break; 
      }
    }
  }
  else{
    for(int i=NUMBER_OF_VOICES;i>=0;i--){
      if(voiceUse[i]==255) {
        voiceUse[i]=note, use=i;
        break; 
      }
    } 
  }
  if(use<NUMBER_OF_VOICES) return use;




}
void resetBuffer(){
  for(int i=0;i<BUFFER_SIZE-1;i++) midiBuffer[i]=0;
  notesInBuffer=0;
}

void freeAllVoices(){
  for(int i=0;i<NUMBER_OF_VOICES;i++) voiceUse[i]=255;
}

void playNotesFromBuffer(){
  //freeAllVoices();
  for(int i=0;i<notesInBuffer;i++){
    if(i<POLYPHONY) proceedNoteOn(midiBuffer[i],ARP_VELOCITY);
  }

}


void putNoteIn(unsigned char note){ 
  putNoteOut(note);
  // check if the note is already in the buffer if yes put it to the first position
  if(notesInBuffer<BUFFER_SIZE){
    if(notesInBuffer>0){ 
      shiftBufferRight();
    }

    midiBuffer[0]=note; // put the last note to the first place 
    notesInBuffer++;
    if(notesInBuffer>POLYPHONY-1) freeVoice(midiBuffer[POLYPHONY]);
    thereIsNoteToPlay=true;
    fromBuffer=0;
  }
}



unsigned char putNoteOut(unsigned char note){
unsigned char takenOut=0;
    boolean takeOut=false;
  if(notesInBuffer>0){ 
    

    for(int i=0;i<notesInBuffer;i++){
      if(midiBuffer[i]==note) takeOut=true, takenOut=i;
    } 

    if(takeOut){
      shiftBufferLeft(takenOut);
      notesInBuffer--;
      for(int i=notesInBuffer;i<BUFFER_SIZE;i++) midiBuffer[i]=0;
      if(takenOut<POLYPHONY && notesInBuffer>=POLYPHONY) {
        thereIsNoteToPlay=true, fromBuffer=POLYPHONY-1;
      }
      return freeVoice(note);

    }

  }


  // put the last note to the first place 

}



void HandleNoteOn(byte channel, byte note, byte velocity) { 
  if(channel==inputChannel){
    hw.setColor(WHITE);
    if (velocity == 0) {
      HandleNoteOff(channel,note,velocity);

    }
    else{
      if(note<8){
        playSound(note,velocity);
        midiSound=note%6;
      }

      else{
        putNoteIn(note);
        orderBuffer();
        if(gateArp && isThereNoteToPlay()) proceedNoteOn(noteToPlay(), velocity);
        //  midiSound=note%6;

      }
    }
  }
}

unsigned char proceedNoteOn(unsigned char _note, unsigned char velocity){

  // unsigned char _note=noteToPlay();
  unsigned char voice=getFreeVoice(_note);
  //  ADSR[voice].noteOff();
  playSound(sound,voice,_note,velocity);
  return voice;

}

void HandleNoteOff(byte channel, byte note, byte velocity){


  if(channel==inputChannel){

    if(note<8){
      unsigned char voice=note%3;
      unsigned char _sound=note%6;

      //if(currentSound[voice]==_sound) 
      ADSR[voice].noteOff();
    }
    else{
      unsigned char outVoice=putNoteOut(note);
      if(outVoice<NUMBER_OF_VOICES && !sustainPedal) ADSR[outVoice].noteOff();

      if(!sustainPedal) orderBuffer();

      if(gateArp && isThereNoteToPlay()) proceedNoteOn(noteToPlay(), ARP_VELOCITY); 
      /*
        else{
       if(outVoice<NUMBER_OF_VOICES) ADSR[outVoice].noteOff();
       }
       */


      //  if(notesInBuffer==0) for(int i=0;i<NUMBER_OF_VOICES;i++) ADSR[i].noteOff(),voiceUse[i]=255; //pojistka

    }

  }
}

#define MOD_WHEEL_BYTE 1
#define SUSTAIN_PEDAL_BYTE 64
#define PRESET_BY_CC_BYTE 0
#define RANDOMIZE_BYTE 127

#define CONTROL_CHANGE_BITS 7
#define CONTROL_CHANGE_OFFSET 3
#define CONTROL_CHANGE_OFFSET_2 102
#define ARP_SWITCH 114
#define ALL_NOTES_OFF 123
#define ALL_SOUNDS_OFF 120

void HandleControlChange(byte channel, byte number, byte value){
  // implement knob movement
  if(channel==inputChannel){
    midiSound=sound;
    if(number==PRESET_BY_CC_BYTE) loadPreset(map(value,0,128,0,NUMBER_OF_PRESETS)), hw.freezeAllKnobs();
    else if(number==SUSTAIN_PEDAL_BYTE){
      if(value>>6){
        sustainPedal=true;
      }

      else{
        for(int i=0;i<NUMBER_OF_VOICES;i++) if(voiceUse[i]==255) ADSR[i].noteOff();
        orderBuffer();
        sustainPedal=false;

      }
    }

    else if (number==ARP_SWITCH){ 
      arp=value>>6;
      if(arp) seq.play(); //sendAllNoteOff(), 
      else turnArpOff();
    }

    else if(number==RANDOMIZE_BYTE) randomize(midiSound);
    else if(number==MOD_WHEEL_BYTE) setVar(midiSound,LFO_AMT,scale(value,CONTROL_CHANGE_BITS,variableDepth[LFO_AMT])), lfoAmt=getVar(midiSound,LFO_AMT)<<1;  

    else if(number>=CONTROL_CHANGE_OFFSET_2 && number<=(CONTROL_CHANGE_OFFSET_2+NUMBER_OF_VARIABLES)){
      number=number-CONTROL_CHANGE_OFFSET_2;
      setVar(midiSound,number,scale(value,CONTROL_CHANGE_BITS,variableDepth[number]));  
      hw.freezeAllKnobs();
      setAllValues(sound);
//      renderTweaking(sound,(number)/VARIABLES_PER_PAGE);
    }
    
    if((number==ALL_NOTES_OFF) || (number==ALL_SOUNDS_OFF)){
      //for(int i=0;i<NUMBER_OF_VOICES;i++) ADSR[i].noteOff();
      sendAllNoteOff();
      freeAllVoices(); 
      resetEnvelope();
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

  if(channel==inputChannel){
    pitchBend=bend;//int(bend-64);
    if(pitchBend==0){
      for(int i=0;i<NUMBER_OF_VOICES;i++)  aOsc[i].setFreq(freq[i]);
    }
    else{
      for(int i=0;i<NUMBER_OF_VOICES;i++) aOsc[i].setFreq((float)map(pitchBend,-8191,8192,(float)mtof(noteVoice[i]-bendRange),(float)mtof(noteVoice[i]+bendRange) )); //freq[i]+
    }
  }
  //centre 8192
  //max 16383
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

void sendAllNoteOff(){
  // for(int i=0;i<NUMBER_OF_VOICES;i++) HandleNoteOff(inputChannel,voiceUse[i],0);//ADSR[i].noteOff(); 
  for(int i=0;i<notesInBuffer;i++) HandleNoteOff(inputChannel,midiBuffer[i],0);
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

  //MIDI.turnThruOn(Full);
  // MIDI.turnThruOff();  
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
  hw.setLed(_channel-1-highChannel*9,false);
  hw.update();
}

#define BASTL_BYTE 0x7D
#define CHANNEL_BYTE 0x00
#define INSTRUMENT_BYTE 0X01 // poly 1.0
#define TEST_BYTE 0x0A
#define NUMBER_OF_MESSAGE_BYTES 2

//unsigned char sysExArray[NUMBER_OF_SYSEX_BYTES];
void HandleSystemExclusive(byte *array, byte size){

  if(array[1]==BASTL_BYTE){ 

    if(array[2]==CHANNEL_BYTE){
      inputChannel=array[2]; 
      array[2]++;
      MIDI.sendSysEx(size,array);
    }
   
    else if(array[2]==TEST_BYTE) test=true, MIDI.turnThruOff(), MIDI.sendSysEx(NUMBER_OF_MESSAGE_BYTES,array,false);
  }
}
