



unsigned char page;
unsigned char sound,lastSound;
boolean midiNote;
#define NUMBER_OF_PAGES 4
#define KNOB_MODE SMALL_BUTTON_2
#define DEFAULT_VELOCITY 127

boolean combo;
int var[NUMBER_OF_VOICES][6];
float pitchMod[NUMBER_OF_VOICES];
unsigned char modDepth[NUMBER_OF_VOICES];
boolean fm[NUMBER_OF_VOICES];
boolean shift;
unsigned char destination[NUMBER_OF_VOICES];
unsigned char ADSRAmt[NUMBER_OF_VOICES];
boolean ADSRDest[NUMBER_OF_VOICES];
unsigned char currentSound[NUMBER_OF_VOICES];
//long lastTicks;


void UI(){

  if(test && pokemon) MIDI.sendControlChange(1,2,3);
  else if(test==true) testMode();
  else if(pokemon==true) pokemonMode();

  else{
    //  MIDI.sendControlChange(1,2,3);


    if(page<3) hw.setColor(page+1);
    else hw.setColor(WHITE);
    renderSmallButtons();
    renderCombo();
    renderKnobs();
    renderBigButtons();
    renderEnvelope();
    renderTweaking(page);
    renderLfo();



  }

}

void setAllValues(unsigned char _SOUND,unsigned char voice){
  var[voice][OSC_FREQ]=float(mtof(byte(getVar(_SOUND,OSC_FREQ))));

  aOsc[voice].setFreq(var[voice][OSC_FREQ]);

  aOsc[voice].setTable(WAVE_TABLES[getBits(getVar(_SOUND,OSC_CHAR),2,2)]);

  fm[voice]=getBits(getVar(_SOUND,OSC_CHAR),4,1);

  if(fm[voice]){

    aMod[voice].setTable(WAVE_TABLES[getBits(getVar(_SOUND,OSC_CHAR),0,2)]);
    var[voice][MOD_FREQ]=float(var[voice][OSC_FREQ]*((float)getVar(_SOUND,MOD_FREQ)/64));
    aMod[voice].setFreq(var[voice][MOD_FREQ]);
    var[voice][MOD_AMT]=getVar(_SOUND,MOD_AMT);
    modDepth[voice]=var[voice][MOD_AMT];

  }
  else{

    var[voice][MOD_FREQ]=getVar(_SOUND,MOD_FREQ);
    var[voice][MOD_AMT]=getVar(_SOUND,MOD_AMT);
    lpf[voice].setCentreFreq(getVar(_SOUND,MOD_FREQ)<<3);
    lpf[voice].setResonance(getVar(_SOUND,MOD_AMT)>>1);
    lpf[voice].setFilterType(getBits(getVar(_SOUND,OSC_CHAR),0,2));

  }

  LFO[voice].setAll(getVar(_SOUND,RATE),getBits(getVar(_SOUND,LFO_CHAR),0,3),getVar(_SOUND,RESOLUTION));
  var[voice][LFO_AMT]=getVar(_SOUND,LFO_AMT);
  destination[voice]=0;
  //  LFO[voice].setShape(getBits(getVar(currentSound[voice],LFO_CHAR),0,3));
  bitWrite(destination[voice],getBits(getVar(_SOUND,LFO_CHAR),3,2),true);
  LFO[voice].reset();



  ADSR[voice].setAttack(scale(getVar(_SOUND,ATTACK),variableDepth[ATTACK],ADSR_BITS));

  if(getVar(_SOUND,SUSTAIN)==0){
    ADSR[voice].setDecay(scale(getVar(_SOUND,RELEASE),variableDepth[RELEASE],SUSTAIN_BITS)); 
  }
  else{
    ADSR[voice].setDecay(scale(getBits(getVar(_SOUND,ADSR_CHAR),5,3),3,ADSR_BITS));
  }
  ADSR[voice].setSustain(scale(getVar(_SOUND,SUSTAIN),variableDepth[SUSTAIN],SUSTAIN_BITS));
  ADSR[voice].setRelease(scale(getVar(_SOUND,RELEASE),variableDepth[RELEASE],SUSTAIN_BITS)); // playDec
  ADSRDest[voice]=getBits(getVar(_SOUND,ADSR_CHAR),0,1);
  ADSRAmt[voice]=getBits(getVar(_SOUND,ADSR_CHAR),1,4)<<4;

}

void playSound(unsigned char _SOUND, unsigned char _VELOCITY){

  unsigned char voice=_SOUND%3;
  currentSound[voice]=_SOUND;
  setAllValues(_SOUND,voice);
  midiNote=false;
  ADSR[voice].noteOff();
  ADSR[voice].noteOn(_VELOCITY);

}


void playSound(unsigned char _SOUND, unsigned char voice, unsigned char _PITCH, unsigned char _VELOCITY){

  //  unsigned char voice=_SOUND%3;
  currentSound[voice]=_SOUND;
  setAllValues(_SOUND,voice);
  var[voice][OSC_FREQ]=float(mtof(_PITCH));
  aOsc[voice].setFreq(var[voice][OSC_FREQ]);
  midiNote=true;
  ADSR[voice].noteOff();
  ADSR[voice].noteOn(_VELOCITY);

}


void renderEnvelope(){

  for(int voice=0;voice<NUMBER_OF_VOICES;voice++){
    ADSR[voice].update();
  }

}
void renderTweaking(unsigned char _page){

  for(int voice=0;voice<NUMBER_OF_VOICES;voice++){
    //tweaking
    if(ADSR[voice].active()){
      switch(_page){
      case OSC_PAGE:
        if(!midiNote) var[voice][OSC_FREQ]=float(mtof(byte(getVar(currentSound[voice],OSC_FREQ))));
        if(fm[voice]){
          var[voice][MOD_FREQ]=float(var[voice][OSC_FREQ]*((float)getVar(currentSound[voice],MOD_FREQ)/64));
          var[voice][MOD_AMT]=getVar(currentSound[voice],MOD_AMT);
          modDepth[voice]=var[voice][MOD_AMT];

        }
        else{
          var[voice][MOD_FREQ]=getVar(currentSound[voice],MOD_FREQ);
          var[voice][MOD_AMT]=getVar(currentSound[voice],MOD_AMT);
        }

        break;
      case LFO_PAGE:
        LFO[voice].setAll(getVar(currentSound[voice],RATE),getBits(getVar(currentSound[voice],LFO_CHAR),0,3),getVar(currentSound[voice],RESOLUTION));
        var[voice][LFO_AMT]=getVar(currentSound[voice],LFO_AMT);
        break;

      case CHAR_PAGE:
        // knob osc
        fm[voice]=getBits(getVar(currentSound[voice],OSC_CHAR),4,1);
        aOsc[voice].setTable(WAVE_TABLES[getBits(getVar(currentSound[voice],OSC_CHAR),2,2)]);
        if(fm[voice]){
          aMod[voice].setTable(WAVE_TABLES[getBits(getVar(currentSound[voice],OSC_CHAR),0,2)]);
        }
        else{
          lpf[voice].setFilterType(getBits(getVar(currentSound[voice],OSC_CHAR),0,2));
        }
        // knob lfo
        LFO[voice].setShape(getBits(getVar(currentSound[voice],LFO_CHAR),0,3));
        destination[voice]=0;
        bitWrite(destination[voice],getBits(getVar(currentSound[voice],LFO_CHAR),3,2),true);
        // knob adsr
        ADSRDest[voice]=getBits(getVar(currentSound[voice],ADSR_CHAR),0,1);
        ADSRAmt[voice]=getBits(getVar(currentSound[voice],ADSR_CHAR),1,4)<<4;
        break;
      }
    }
  }
  //tweaking end 
}
void renderLfo(){

  for(int voice=0;voice<NUMBER_OF_VOICES;voice++){

    if(ADSR[voice].active()){
      LFO[voice].update();

      if(ADSRDest[voice])  aOsc[voice].setFreq(var[voice][OSC_FREQ] + isDestination(voice,OSC_FREQ)*((LFO[voice].next()*var[voice][LFO_AMT])>>8)+((ADSR[voice].next() * ADSRAmt[voice])>>8));
      else aOsc[voice].setFreq(var[voice][OSC_FREQ] + isDestination(voice,OSC_FREQ)*((LFO[voice].next()*var[voice][LFO_AMT])>>8));

      if(fm[voice]){
        if(!ADSRDest[voice])  aMod[voice].setFreq(var[voice][MOD_FREQ]+ isDestination(voice,MOD_FREQ)*((LFO[voice].next()*var[voice][LFO_AMT])>>8)+((ADSR[voice].next() * ADSRAmt[voice])>>8));
        else aMod[voice].setFreq(var[voice][MOD_FREQ]+ isDestination(voice,MOD_FREQ)*((LFO[voice].next()*var[voice][LFO_AMT])>>8));
        modDepth[voice]=var[voice][MOD_AMT]+ isDestination(voice,MOD_FREQ)*((LFO[voice].next()*var[voice][LFO_AMT])>>8);
      }

      else{   
        unsigned char lpfNow;
        if(!ADSRDest[voice]) lpfNow=var[voice][MOD_FREQ] + isDestination(voice,MOD_FREQ)*((LFO[voice].next()*var[voice][LFO_AMT])>>8);
        else lpfNow=var[voice][MOD_FREQ] + isDestination(voice,MOD_FREQ)*((LFO[voice].next()*var[voice][LFO_AMT])>>8)+((ADSR[voice].next() * ADSRAmt[voice])>>8);
        lpf[voice].setCentreFreq(lpfNow<<3);
        lpfNow=var[voice][MOD_AMT] + isDestination(voice,MOD_AMT)*((LFO[voice].next()*var[voice][LFO_AMT])>>8);
        lpf[voice].setResonance(lpfNow>>1);
      }
    }
  }
}
boolean isDestination(unsigned char voice,unsigned char _destination){
  return bitRead(destination[voice],_destination); 
}


void renderSmallButtons(){
  // knobMode
  if(hw.justReleased(KNOB_MODE) && !combo){ 
    if(page<NUMBER_OF_PAGES-1) page++;
    else page=0;
    hw.freezeAllKnobs();
  } 

  if(hw.justPressed(SMALL_BUTTON_1)) shift=!shift;
  if(hw.justReleased(SMALL_BUTTON_1) && !bootShift) shift=!shift;
  if(hw.justPressed(EXTRA_BUTTON_1)) shift=!shift;


}


#define LONG_THRESHOLD 40
boolean countLong, longPress;
int longCount;
void renderCombo(){

  //loading saving presets
  // if(hw.buttonState(SMALL_BUTTON_2) && hw.justPressed(SMALL_BUTTON_1)) loadPreset(0), hw.freezeAllKnobs(), combo=true;
  if(countLong) {
    longCount++;
    if(longCount>LONG_THRESHOLD) longPress=true;
  }

  for(int i=0;i<3;i++){
    if(hw.buttonState(EXTRA_BUTTON_2) && hw.justPressed(i)) randomize(i+3*shift); // randomize 
    if(hw.buttonState(SMALL_BUTTON_1) && hw.buttonState(SMALL_BUTTON_2) && hw.justPressed(i)) countLong=true,longCount=0, combo=true; // save&load
    if(hw.buttonState(SMALL_BUTTON_1) && hw.buttonState(SMALL_BUTTON_2) && hw.justReleased(i)){
      if(longPress) storePreset(currentPreset),loadPreset(i+3), hw.freezeAllKnobs(), combo=true; // save&load
      else storePreset(currentPreset), loadPreset(i+3*hw.buttonState(EXTRA_BUTTON_2)), hw.freezeAllKnobs(), combo=true;
     // countLong=false, longCount=0,longPress=false;
    }

    if(hw.buttonState(SMALL_BUTTON_2) && hw.justPressed(i)) countLong=true,longCount=0, combo=true; // load
    if(hw.buttonState(SMALL_BUTTON_2) && hw.justReleased(i)){
      if(longPress) loadPreset(i+3), hw.freezeAllKnobs();
      else loadPreset(i+3*hw.buttonState(EXTRA_BUTTON_2)), hw.freezeAllKnobs();
     // countLong=false, longCount=0, longPress=false;
    }
  }

  if(combo){
    //turn off combo when all buttons are released 
    unsigned char _count=0; 
    for(int i=0;i<NUMBER_OF_BUTTONS;i++)  _count+=hw.buttonState(i); // if(!hw.buttonState(i)) combo=false;
    if(_count==0) combo=false,countLong=false, longCount=0,longPress=false;
    //else combo=true;
  } 


}


void randomize(unsigned char _sound){
  for(int i=0;i<NUMBER_OF_VARIABLES;i++) setVar(_sound,i,rand(maxValue[i])), hw.freezeAllKnobs();
}
void renderBigButtons(){

  for(int i=0;i<NUMBER_OF_BIG_BUTTONS;i++){
    //if(voiceUse[i]<255) hw.setLed(i,true);
    // else hw.setLed(i,false);

    if(shift){
      if(currentSound[i]==i+NUMBER_OF_BIG_BUTTONS && ADSR[i].active()) hw.setLed(i,true);
      else hw.setLed(i,false);

    }
    else{
      if(currentSound[i]==i && ADSR[i].active()) hw.setLed(i,true);
      else hw.setLed(i,false);
      // if(currentSound[i]==i) hw.setLed(i,ADSR[i].active());
    }
    if(longPress) hw.setLed(i,true);
  }

  if(!combo){
    for(int i=BIG_BUTTON_1;i<=BIG_BUTTON_3;i++) {

      if(hw.justPressed(i)) {
        if(!shift) sound=i, playSound(sound,DEFAULT_VELOCITY);
        else sound=i+NUMBER_OF_BIG_BUTTONS, playSound(sound,DEFAULT_VELOCITY);
      }
      if(hw.justReleased(i))  ADSR[i].noteOff();
    };
    if(sound!=lastSound)  hw.freezeAllKnobs();
    lastSound=sound;
  } 

}
void renderKnobs(){

  for(int i=0;i<NUMBER_OF_KNOBS;i++){

    unsigned char _variable=i+VARIABLES_PER_PAGE*page;


    if(hw.knobFreezed(i)) {
      if(lastSound==sound){ // knobs are unfreezed when they come across their original value
        if(inBetween( scale(hw.knobValue(i),KNOB_BITS,variableDepth[_variable]), scale(hw.lastKnobValue(i),KNOB_BITS,variableDepth[_variable]),getVar(sound,_variable) ) ){
          hw.unfreezeKnob(i); //external unfreez

          if(page<3) hw.setColor(WHITE);
          else hw.setColor(BLACK);
        }
      }  
    }

    else{       
      setVar(sound,_variable,scale(hw.knobValue(i),KNOB_BITS,variableDepth[_variable])); 
    }
  }
}

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
#define ANIMATION_DELAY 150
void animation(){

  hw.setLed(LED_1,true);
  hw.update();
  delay(ANIMATION_DELAY);
  hw.setLed(LED_3,true);
  hw.update();
  delay(ANIMATION_DELAY);
  hw.setLed(LED_2,true);
  hw.update();
  delay(ANIMATION_DELAY);
  hw.setColor(BLUE);
  hw.update();
  delay(ANIMATION_DELAY);
  hw.setColor(WHITE);
  hw.update();
  delay(ANIMATION_DELAY);
  hw.setColor(BLACK);
  hw.setLed(LED_1,false);
  hw.setLed(LED_2,false);
  hw.setLed(LED_3,false);
  hw.update();

}







#define NUMBER_OF_MESSAGE_BYTES 6
#define NUMBER_OF_SYSEX_BYTES 4
unsigned char sysExArray[NUMBER_OF_SYSEX_BYTES];

#define SWITCH_BYTE 14
#define PAGE_BYTE 13

#define BASTL_BYTE 0x7D
#define CHANNEL_BYTE 0x00
#define INSTRUMENT_BYTE 0X01 // poly 1.0
#define TEST_BYTE 0x0A

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

void sendSysExArray(){
  MIDI.sendSysEx(NUMBER_OF_MESSAGE_BYTES,sysExArray,false);
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













