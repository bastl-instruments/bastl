



unsigned char midiSound;

#define POLYPHONY NUMBER_OF_VOICES
unsigned char notesInBuffer=ZERO;
boolean thereIsNoteToPlay;

#define BUFFER_SIZE 16
unsigned char midiBuffer[BUFFER_SIZE];
#define DEFAULT_VOICE_USE 255

unsigned char voiceUse[NUMBER_OF_VOICES]={
  DEFAULT_VOICE_USE,DEFAULT_VOICE_USE,DEFAULT_VOICE_USE};

unsigned char fromBuffer;
boolean ping;


void shiftBufferLeft(unsigned char from){
  for(int i=from;i<notesInBuffer;i++){
    midiBuffer[i]=midiBuffer[i+1]; 
  }
}

void shiftBufferRight(){
  for(int i=notesInBuffer;i>ZERO;i--){
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
  for(int i=ZERO;i<NUMBER_OF_VOICES;i++){
    if(voiceUse[i]==note){
      voiceUse[i]=255, use=i;
      break;
    }
  }
  return use;

}

unsigned char getFreeVoice(unsigned char note){

  unsigned char use=255;
  for(int i=ZERO;i<NUMBER_OF_VOICES;i++){
    if(voiceUse[i]==255) {
      voiceUse[i]=note, use=i;
      break; 
    }
  }

  if(use<NUMBER_OF_VOICES) return use;


}


void putNoteIn(unsigned char note){

  // check if the note is already in the buffer if yes put it to the first position
  if(notesInBuffer<BUFFER_SIZE){
    if(notesInBuffer>ZERO){ 
      shiftBufferRight();
    }

    midiBuffer[ZERO]=note; // put the last note to the first place 
    notesInBuffer++;
    if(notesInBuffer>POLYPHONY-1) freeVoice(midiBuffer[POLYPHONY]);
    thereIsNoteToPlay=true;
    fromBuffer=ZERO;
  }
}



unsigned char putNoteOut(unsigned char note){

  if(notesInBuffer>ZERO){ 
    unsigned char takenOut;
    boolean takeOut=ZERO;

    for(int i=ZERO;i<notesInBuffer;i++){
      if(midiBuffer[i]==note) takeOut=true, takenOut=i;
    } 

    if(takeOut){
      shiftBufferLeft(takenOut);
      notesInBuffer--;
      for(int i=notesInBuffer;i<BUFFER_SIZE;i++) midiBuffer[i]=ZERO;
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
    if (velocity == ZERO) {
      HandleNoteOff(channel,note,velocity);

    }
    else{
      if(note<6){
        playSound(note,velocity);
        midiSound=note%6;
      }

      else{
        putNoteIn(note);
        if(isThereNoteToPlay()) {
          unsigned char _note=noteToPlay();
          unsigned char voice=getFreeVoice(_note);
          //  ADSR[voice].noteOff();
          playSound(sound,voice,_note,velocity);
        }

        //  midiSound=note%6;


      }
    }
  }
}

void HandleNoteOff(byte channel, byte note, byte velocity){


  if(channel==inputChannel){

    if(note<6){
      unsigned char voice=note%3;
      unsigned char _sound=note%6;

      if(currentSound[voice]==_sound) ADSR[voice].noteOff();
    }
    else{
      unsigned char outVoice=putNoteOut(note);

      if(isThereNoteToPlay()) {
        unsigned char _note=noteToPlay();
        unsigned char voice=getFreeVoice(_note);
        ADSR[outVoice].noteOff();
        // =getFreeVoice(_note);
        playSound(sound,voice,_note,velocity);
      }
      else{
        if(outVoice<NUMBER_OF_VOICES) ADSR[outVoice].noteOff();
      }

      //  if(notesInBuffer==0) for(int i=0;i<NUMBER_OF_VOICES;i++) ADSR[i].noteOff(),voiceUse[i]=255; //pojistka

    }

  }
}

#define MOD_WHEELL_BYTE 1
#define SUSTAIN_PEDAL_BYTE 64
#define PRESET_BY_CC_BYTE 0
#define RANDOMIZE_BYTE 127

#define CONTROL_CHANGE_BITS 7
#define CONTROL_CHANGE_OFFSET 3
#define CONTROL_CHANGE_OFFSET_2 102

boolean sustainPedal;
//#define SUSTAIN_PEDAL_BYTE 64
//#define NUMBER_OF_REAL_VARIABLES 20

void HandleControlChange(byte channel, byte number, byte value){
  // implement knob movement
  if(channel==inputChannel){

    if(number==PRESET_BY_CC_BYTE) loadPreset(map(value,0,128,0,NUMBER_OF_PRESETS)), hw.freezeAllKnobs();
    
    else if(number==RANDOMIZE_BYTE) randomize(midiSound);
    
    else if(number==SUSTAIN_PEDAL_BYTE) sustainPedal=value>>6;

    else if(number>=CONTROL_CHANGE_OFFSET && number<76){
      number=number-CONTROL_CHANGE_OFFSET;
      if(number>64) number--;
      number=number-CONTROL_CHANGE_OFFSET;
      midiSound=number/NUMBER_OF_VARIABLES;
      number=number%NUMBER_OF_VARIABLES;
      setVar(midiSound,number,scale(value,CONTROL_CHANGE_BITS,variableDepth[number]));  
      hw.freezeAllKnobs();
      renderTweaking((number)/VARIABLES_PER_PAGE);
    }

    else if(number>=CONTROL_CHANGE_OFFSET_2 && number<=(CONTROL_CHANGE_OFFSET_2+NUMBER_OF_VARIABLES)){
      number=number-CONTROL_CHANGE_OFFSET_2;
      setVar(midiSound,number,scale(value,CONTROL_CHANGE_BITS,variableDepth[number]));  
      hw.freezeAllKnobs();
      renderTweaking((number)/VARIABLES_PER_PAGE);
    }

  }

  //testMode
  if(test){
    if(number==SWITCH_BYTE){
      for(int i=0;i<3;i++) hw.setSwitch(i,bitRead(value,i));
    }
    else if(number==PAGE_BYTE){
      page=value;
    }
  }

}

void HandleProgramChange(byte channel, byte number  ){
  // implement preset change
}

void HandlePitchBend(byte channel, int bend){

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

  MIDI.begin(ZERO);    
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

#define SECOND_LAYER_CHANNEL 10
void indicateMidiChannel(unsigned char _channel){

  boolean highChannel=false;
  if(_channel>=SECOND_LAYER_CHANNEL)   hw.setLed(LED_1,true), hw.setLed(LED_2,true), hw.setLed(LED_3,true), highChannel=true;
  for(int i=ZERO;i<NUMBER_OF_KNOBS;i++){
    hw.setLed(_channel-1-highChannel*9,false);
    hw.update();
    delay(ANIMATION_DELAY);
    hw.setLed(_channel-1-highChannel*9,true);
    hw.update();
    delay(ANIMATION_DELAY);
  }

}





































