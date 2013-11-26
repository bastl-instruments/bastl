boolean pokemon=false;
unsigned char page;
boolean test;
boolean midiNote;
#define NUMBER_OF_PAGES 3
#define KNOB_MODE SMALL_BUTTON_2
#define DEFAULT_VELOCITY 127
#define NUMBER_OF_ARP_TYPES 9
boolean slave=false;
boolean sustainPedal;

boolean combo;
//int var[NUMBER_OF_VARIABLES];
//unsigned char currentSound[NUMBER_OF_VOICES];
#define NUMBER_OF_DIVIDERS 8
unsigned char dividers[NUMBER_OF_DIVIDERS]={
  // 0,2,5,11,23,47,95,191};
  1,3,6,12,24,48,96,192};
long lastTicks;
unsigned char xorAmt;
unsigned char xorType;
unsigned char lfoAmt;
boolean arp;
unsigned char arpType;
boolean gate=true;
int pitchBend;
unsigned char bendRange=2;
boolean gateArp=true;

void UI(){

  //if(pokemon) pokemonMode();

  //else{
  if(test) testMode();
  else{
    hw.setColor(page+1);


    renderSmallButtons();
    renderCombo();
    renderBigButtons();
    renderKnobs();
    renderEnvelope();
    renderTweaking(sound,page);
    renderLfo();

    if(arp) renderArp();
  }
  //for(int i=0;i<3;i++) hw.setLed(i,ADSR[i].active());
  //if(sustainPedal) hw.setColor(WHITE);
  // }

}

void setAllValues(unsigned char _SOUND){


  // for(int i=0;i<NUMBER_OF_VARIABLES;i++) var[i]=getVar(_SOUND,i);

  unsigned char wt=getBits(getVar(_SOUND,OSC_CHAR),0,2);
  for(int i=0;i<NUMBER_OF_VOICES;i++) aOsc[i].setTable(WAVE_TABLES[wt]); 
  //  xorType=map(getBits(getVar(_SOUND,OSC_CHAR),2,3),0,8,0,5);
  xorType=getBits(getVar(_SOUND,OSC_CHAR),2,2);
  if(wt==3 && xorType==3) xorType=4;
  xorAmt=getVar(_SOUND,XOR_AMT)<<1;



  arpType=getVar(_SOUND,ARP_TYPE);


  if(!slave){
    seq.setTempo(120);//5+getVar(_SOUND,ARP_RATE));
    seq.setClockDivider(65-(getVar(_SOUND,ARP_RATE)>>1));
  }
  else seq.setClockDivider(dividers[8-(getVar(_SOUND,ARP_RATE)>>4)]);

  LFO.setAll(getVar(_SOUND,LFO_RATE)<<3,getVar(_SOUND,LFO_SHAPE),getVar(_SOUND,LFO_RES)); //
  lfoAmt=getVar(_SOUND,LFO_AMT)<<1;
  //LFO.reset();

  for(int i=0;i<NUMBER_OF_VOICES;i++) {
    ADSR[i].setAttack(scale(getVar(_SOUND,ATACK),variableDepth[ATACK],ADSR_BITS)) ;
    ADSR[i].setDecay(scale(getVar(_SOUND,DECAY),variableDepth[DECAY],ADSR_BITS)) ;
    ADSR[i].setSustain(scale(getVar(_SOUND,SUSTAIN),variableDepth[SUSTAIN],SUSTAIN_BITS));
    ADSR[i].setRelease(scale(getVar(_SOUND,RELEASE),variableDepth[RELEASE],ADSR_BITS));
  }

}


void playSound(unsigned char _SOUND, unsigned char _VELOCITY){

  unsigned char voice=_SOUND%3;
  //  currentSound[voice]=_SOUND;
  setAllValues(_SOUND);
  midiNote=false;
  //ADSR[voice].noteOff();
  // ADSR[voice].noteOn(_VELOCITY);


}


float freq[NUMBER_OF_VOICES];
unsigned char noteVoice[NUMBER_OF_VOICES];

void playSound(unsigned char _SOUND, unsigned char voice, unsigned char _PITCH, unsigned char _VELOCITY){

  //  unsigned char voice=_SOUND%3;
  // currentSound[voice]=_SOUND;


  // setAllValues(_SOUND); // nemusi byt
  noteVoice[voice]=_PITCH;
  freq[voice]=float(mtof(_PITCH)); //pitchBend; mtof(_PITCH)
  aOsc[voice].setFreq(freq[voice]);

  if(pitchBend==0){
    aOsc[voice].setFreq(freq[voice]);
  }
  else{
    aOsc[voice].setFreq((float)map(pitchBend,-8191,8192,(float)mtof(noteVoice[voice]-bendRange),(float)mtof(noteVoice[voice]+bendRange) )); //freq[i]+
  }

  midiNote=true;
  ADSR[voice].noteOff();
  ADSR[voice].noteOn(_VELOCITY);

}


void renderEnvelope(){

  for(int voice=0;voice<NUMBER_OF_VOICES;voice++){
    ADSR[voice].update();
  }

}



void renderTweaking(unsigned char _SOUND, unsigned char _page){


  //tweaking

  switch(_page){

  case OSC_PAGE:

    if(!hw.knobFreezed(RIGHT_KNOB) && hw.knobMoved(RIGHT_KNOB)){

      unsigned char wt=getBits(getVar(_SOUND,OSC_CHAR),0,2);
      for(int i=0;i<NUMBER_OF_VOICES;i++) aOsc[i].setTable(WAVE_TABLES[wt]); 
      // xorType=map(getBits(getVar(_SOUND,OSC_CHAR),2,3),0,8,0,5);
      xorType=getBits(getVar(_SOUND,OSC_CHAR),2,2);
      if(wt==3 && xorType==3) xorType=4;
    }
    if(!hw.knobFreezed(LEFT_KNOB)&& hw.knobMoved(LEFT_KNOB)) xorAmt=getVar(_SOUND,XOR_AMT)<<1;

    if(!hw.knobFreezed(TOP_KNOB)&& hw.knobMoved(TOP_KNOB)) {

      //  seq.setTempo(40+getVar(_SOUND,ARP_RATE)<<1);
      if(!slave){
        seq.setTempo(120);//5+getVar(_SOUND,ARP_RATE));
        seq.setClockDivider(65-(getVar(_SOUND,ARP_RATE)>>1)); 
      }
      else seq.setClockDivider(dividers[8-(getVar(_SOUND,ARP_RATE)>>4)]);

    }

    break;

  case LFO_PAGE:
    LFO.setAll(getVar(_SOUND,LFO_RATE)<<3,getVar(_SOUND,LFO_SHAPE),getVar(_SOUND,LFO_RES));
    if(!hw.knobFreezed(LEFT_KNOB) && hw.knobMoved(LEFT_KNOB)) lfoAmt=getVar(_SOUND,LFO_AMT)<<1;
    break;

  case ADSR_PAGE:
    for(int i=0;i<NUMBER_OF_VOICES;i++) {
      if(!hw.knobFreezed(TOP_KNOB) && hw.knobMoved(TOP_KNOB)) ADSR[i].setAttack(scale(getVar(_SOUND,ATACK),variableDepth[ATACK],ADSR_BITS)) ;
      if(!hw.knobFreezed(LEFT_KNOB) && hw.knobMoved(LEFT_KNOB)) ADSR[i].setSustain(scale(getVar(_SOUND,SUSTAIN),variableDepth[SUSTAIN],SUSTAIN_BITS));
      if(!hw.knobFreezed(RIGHT_KNOB) && hw.knobMoved(RIGHT_KNOB))  ADSR[i].setRelease(scale(getVar(_SOUND,RELEASE),variableDepth[RELEASE],ADSR_BITS));
    }
    break;
  }


  //tweaking end 

}
int lfoNow;
int amplitude;
void renderLfo(){

  LFO.update();
  lfoNow=255-((lfoAmt*LFO.next())>>8);// 
  amplitude=(lfoNow-(xorAmt>>7)*50)*gate;
}

/*
boolean isDestination(unsigned char voice,unsigned char _destination){
 return bitRead(destination[voice],_destination); 
 }
 */
void resetEnvelope(){
  for(int i=0;i<NUMBER_OF_VOICES;i++) ADSR[i].noteOff(); 
}


void renderSmallButtons(){
  // knobMode
  if(hw.justReleased(KNOB_MODE) && !combo){ 
    if(page<NUMBER_OF_PAGES-1) page++;
    else page=0;
    hw.freezeAllKnobs();
  } 
  if(page==OSC_PAGE){
    if(hw.justReleased(SMALL_BUTTON_1) && !combo){

      arp=!arp;
      if(arp) seq.play(); //sendAllNoteOff(), 
      else turnArpOff();
    }
  }
  else{


    if(hw.justReleased(SMALL_BUTTON_1) && !combo){

      if(page==ADSR_PAGE) {
        setVar(sound,DECAY,increaseValue(getVar(sound,DECAY),maxValue[DECAY]));
        for(int i=0;i<NUMBER_OF_VOICES;i++) ADSR[i].setDecay(scale(getVar(sound,DECAY),variableDepth[DECAY],ADSR_BITS)) ;
      }
      if(page==LFO_PAGE){
        setVar(sound,LFO_SHAPE,increaseValue(getVar(sound,LFO_SHAPE),maxValue[LFO_SHAPE]));
        LFO.setShape(getVar(sound,LFO_SHAPE));
      }
    }
  }
}


void turnArpOff(){
  gateArp=true;
  seq.stop();
  gate=true; 
  sendAllNoteOff();
  freeAllVoices(); 
  resetEnvelope();
  playNotesFromBuffer();//,  resetBuffer() playNotesFromBuffer(); 

}

#define LONG_THRESHOLD 80
boolean countLong, longPress;
int longCount;

void renderCombo(){

  if(countLong) {
    longCount++;
    if(longCount>LONG_THRESHOLD) longPress=true;
  }


  for(int i=0;i<3;i++){

    if(hw.buttonState(EXTRA_BUTTON_2) && hw.justPressed(i)) randomize(sound); // randomize 
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

    if(longPress) hw.setLed(i,true);
    else hw.setLed(i,hw.switchState(i));
  }

  if(page==OSC_PAGE && hw.buttonState(SMALL_BUTTON_1) && hw.justPressed(SMALL_BUTTON_2)) arpType=increaseValue(arpType,NUMBER_OF_ARP_TYPES-1),combo=true, setVar(sound,ARP_TYPE,arpType);


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

  if(!combo){
    for(int i=BIG_BUTTON_1;i<=BIG_BUTTON_3;i++) {

      if(hw.justPressed(i)){
        hw.flipSwitch(i), sound=hw.soundFromSwitches();
        // playSound(sound,DEFAULT_VELOCITY);
        hw.setLed(i,hw.switchState(i));
      }
    };
    if(sound!=lastSound)  setAllValues(sound), hw.freezeAllKnobs();
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
          hw.setColor(WHITE);
        }
      }  
    }

    else if(hw.knobMoved(i)) setVar(sound,_variable,scale(hw.knobValue(i),KNOB_BITS,variableDepth[_variable])); 

  }
}

int increaseValue(int _VALUE, int _OVERFLOW){
  if(_VALUE < _OVERFLOW) _VALUE++;
  else _VALUE=0;
  return _VALUE;
}
/*
int decreaseValue(int _VALUE, int _OVERFLOW){
  if(_VALUE > _OVERFLOW) _VALUE--;
  return _VALUE;
}
*/


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

void animation(){

  hw.setLed(LED_1,true);
  hw.update();
  delay(150);
  hw.setLed(LED_3,true);
  hw.update();
  delay(150);
  hw.setLed(LED_2,true);
  hw.update();
  delay(150);
  hw.setColor(BLUE);
  hw.update();
  delay(150);
  hw.setColor(WHITE);
  hw.update();
  delay(150);
  hw.setColor(BLACK);
  hw.setLed(LED_1,false);
  hw.setLed(LED_2,false);
  hw.setLed(LED_3,false);
  hw.update();

}



/*

 
 void pokemonMode(){
 
 if(audioTicks()-lastTicks>4000) lastTicks=audioTicks(), page=increaseValue(page,7);
 hw.setColor(page);
 shift=hw.buttonState(SMALL_BUTTON_1);
 if(hw.buttonState(SMALL_BUTTON_1) && hw.justPressed(SMALL_BUTTON_2)) makeSysExArray(sound), sendSysExArray();
 for(int i=0;i<3;i++) if(hw.buttonState(SMALL_BUTTON_2) && hw.justPressed(i)) loadPreset(i);
 
 renderBigButtons();
 renderEnvelope();
 renderLfo(); 
 }
 
 
 
 
 
 
 void checkForPokemon(){
 if(hw.buttonState(SMALL_BUTTON_1) && hw.buttonState(SMALL_BUTTON_2)) pokemon=true; 
 
 }
 
 
 
 #define NUMBER_OF_MESSAGE_BYTES 16
 #define NUMBER_OF_SYSEX_BYTES 14
 unsigned char sysExArray[NUMBER_OF_SYSEX_BYTES];
 
 #define BASTL_BYTE 0x7D
 #define CHANNEL_BYTE 0x00
 #define INSTRUMENT_BYTE 0X01 // poly 1.0
 
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
 */























