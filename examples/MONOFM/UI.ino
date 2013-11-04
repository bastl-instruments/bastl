unsigned char page;
boolean pokemon=false;
unsigned char sound,lastSound;
long lastTicks;
#define NUMBER_OF_PAGES 5



boolean combo;

void UI(){
  if(test) testMode();
 else if(pokemon) pokemonMode();

  else{
    if(page<4) hw.setColor(page);
    else hw.setColor(WHITE);

    renderSmallButtons();
    renderBigButtons();
    renderKnobs();
    renderCombo();
    //RENDER SOUND
    loadSound(sound);
    renderLfo();
    setValues(); // set values for interupt update - updateAudio()
  }
}

void renderBigButtons(){
  if(!combo){

    for(int i=BIG_BUTTON_1;i<=BIG_BUTTON_3;i++) {
      if(hw.justPressed(i)) hw.flipSwitch(i);
    };

    for(int i=0;i<3;i++) {
      hw.setLed(i,hw.switchState(i));
    }

    lastSound=sound;
    sound=hw.soundFromSwitches();
    if(sound!=lastSound) storeSound(lastSound), loadSound(sound),setTables(sound), hw.freezeAllKnobs();

  } 
}

void renderSmallButtons(){


  if(hw.justReleased(SMALL_BUTTON_2) && !combo){ 
    if(page<NUMBER_OF_PAGES-1) page++;
    else page=0;



    storeSound(lastSound);
    loadSound(sound);
    hw.freezeAllKnobs();
  }

  if(hw.justReleased(SMALL_BUTTON_1) && !combo){

    if(page==CARRIER_PAGE) {
      if(var[CARRIER_WAVETABLE]<NUMBER_OF_WAVETABLES) var[CARRIER_WAVETABLE]++;
      else var[CARRIER_WAVETABLE]=0;
      setVar(sound,CARRIER_WAVETABLE,var[CARRIER_WAVETABLE]);
      aCar.setTable(WAVE_TABLES[getVar(sound,CARRIER_WAVETABLE)]); 
    }
    else if(page == LFO_PAGE) {
      if(var[LFO_WAVETABLE]<NUMBER_OF_LFO_SHAPES) var[LFO_WAVETABLE]++;
      else var[LFO_WAVETABLE]=0;
      setVar(sound,LFO_WAVETABLE,var[LFO_WAVETABLE]);
      LFO.setShape(LFO_WAVETABLE);
    }
    else{
      if(var[4*page+3]<NUMBER_OF_WAVETABLES) var[4*page+3]++;
      else var[4*page+3]=0;
      setVar(sound,4*page+3,var[4*page+3]);
      aMod[page-1].setTable(WAVE_TABLES[getVar(sound,4*page+3)]); 
    }

  } 

}

void renderKnobs(){
  //KNOBS
  for(int i=0;i<3;i++){
    unsigned char _variable=i+4*page;
    if(hw.knobFreezed(i)) { // knobs are unfreezed when they come across their original value
      if(inBetween( scale(hw.knobValue(i),KNOB_BITS,variableDepth[_variable]), scale(hw.lastKnobValue(i),KNOB_BITS,variableDepth[_variable]),getVar(sound,_variable) ) ){
        hw.unfreezeKnob(i);
        if(page<4) hw.setColor(WHITE);
        else hw.setColor(BLACK);
        //external unfreez
      }
    }
    else{ // when the knobs are unfreezed the values are stored in the memory
      setVar(sound,_variable,scale(hw.knobValue(i),KNOB_BITS,variableDepth[_variable])); 
    }
  }
}
void randomize(unsigned char _sound){
  for(int i=0;i<NUMBER_OF_VARIABLES;i++) setVar(_sound,i,rand(maxValue[i])), hw.freezeAllKnobs(),loadSound(_sound);
}

void renderCombo(){

  for(int i=0;i<3;i++){
    if(hw.buttonState(EXTRA_BUTTON_2) && hw.justPressed(i)) randomize(hw.soundFromSwitches()); // randomize 
    if(hw.buttonState(SMALL_BUTTON_1) && hw.buttonState(SMALL_BUTTON_2) && hw.justPressed(i)) storeSound(sound),storePreset(currentPreset), loadPreset(i+3*hw.buttonState(EXTRA_BUTTON_2)), hw.freezeAllKnobs(), loadSound(sound),combo=true; // save&load
    if(hw.buttonState(SMALL_BUTTON_2) && hw.justPressed(i)) loadPreset(i+3*hw.buttonState(EXTRA_BUTTON_2)), hw.freezeAllKnobs(), loadSound(sound),combo=true; // load
  }


  if(combo){

    hw.setLed(LED_1,true); //indicate combo
    hw.setLed(LED_2,true);
    hw.setLed(LED_3,true); 

    //turn off combo when all buttons are released 
    unsigned char _count=0; 
    for(int i=0;i<NUMBER_OF_BUTTONS;i++)  _count+=hw.buttonState(i); // if(!hw.buttonState(i)) combo=false;
    if(_count==0) combo=false;
    //else combo=true;

  }
}

int lfoNow;

void renderLfo(){
  LFO.setAll(var[LFO_RATE],var[LFO_WAVETABLE],getBits(var[LFO_DEST],0,5)<<1);


  //  LFO.setResolution(63);
  //LFO.setRate(var[LFO_RATE]);
  LFO.update();
  lfoNow=(LFO.next()*var[LFO_AMT])>>7;
}

void loadSound(unsigned char _SOUND){
  for(int i=0;i<NUMBER_OF_VARIABLES;i++){
    var[i]=getVar(_SOUND,i);
  }
}

void storeSound(unsigned char _SOUND){
  for(int i=0;i<NUMBER_OF_VARIABLES;i++){
    setVar(_SOUND,i,var[i]);
  }
}
boolean isDestination(unsigned char _DEST){

  if(getBits(var[LFO_DEST],5,3)==_DEST) return true;
  else return false;

}

void setTables(unsigned char _sound){
  aCar.setTable(WAVE_TABLES[getVar(_sound,CARRIER_WAVETABLE)]);
  LFO.setShape(LFO_WAVETABLE);
  for(int i=0;i<NUMBER_OF_MODULATORS;i++) aMod[i].setTable(WAVE_TABLES[getVar(_sound,4+i*page+3)]); 
}

void setValues(){

  float carrier_freq =mtof(byte(var[CARRIER_FREQ]))+lfoNow*isDestination(0);
  float mod_ratio[NUMBER_OF_MODULATORS];

  mod_ratio[MOD_1]=(float)var[MOD_1_HARMONIC]+((float)(var[MOD_1_DETUNE]+(lfoNow>>1)*isDestination(2))/255);
  mod_ratio[MOD_2]=(float)var[MOD_2_HARMONIC]+((float)(var[MOD_2_DETUNE]+(lfoNow>>1)*isDestination(4))/255);
  mod_ratio[MOD_3]=(float)var[MOD_3_HARMONIC]+((float)(var[MOD_3_DETUNE]+(lfoNow>>1)*isDestination(6))/255);

  for(int i=0;i<NUMBER_OF_MODULATORS;i++){
    float mod_freq = carrier_freq * mod_ratio[i];
    aMod[i].setFreq(mod_freq);
  }

  aCar.setFreq(carrier_freq); 

  //mod_amt[MOD_1] =var[MOD_1_AMT]; 

}

void setValuesToZero(){

  int carrier_freq =0;
  float mod_ratio[NUMBER_OF_MODULATORS];

  mod_ratio[MOD_1]=0;//(float)var[MOD_1_HARMONIC]+((float)var[MOD_1_DETUNE]/128);
  mod_ratio[MOD_2]=0;//(float)var[MOD_2_HARMONIC]+((float)var[MOD_1_DETUNE]/128);
  mod_ratio[MOD_3]=0;//(float)var[MOD_3_HARMONIC]+((float)var[MOD_1_DETUNE]/128);

  for(int i=0;i<NUMBER_OF_MODULATORS;i++){
    float mod_freq = carrier_freq * mod_ratio[i];
    aMod[i].setFreq(0);
  }

  aCar.setFreq(carrier_freq); 
  //mod_amt[MOD_1] =var[MOD_1_AMT]; 

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



void pokemonMode(){

  if(audioTicks()-lastTicks>4000) lastTicks=audioTicks(), page=increaseValue(page,7);
  hw.setColor(page);
  //shift=hw.buttonState(SMALL_BUTTON_1);
  if(hw.buttonState(SMALL_BUTTON_1) && hw.justPressed(SMALL_BUTTON_2)) makeSysExArray(sound), sendSysExArray();
  for(int i=0;i<3;i++) if(hw.buttonState(SMALL_BUTTON_2) && hw.justPressed(i)) loadPreset(i);

 //  renderSmallButtons();
    renderBigButtons();
  //  renderKnobs();
   // renderCombo();
    //RENDER SOUND
    loadSound(sound);
    renderLfo();
    setValues();  
}

void checkForPokemon(){
  if(hw.buttonState(SMALL_BUTTON_1) && hw.buttonState(SMALL_BUTTON_2)) pokemon=true; 

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

