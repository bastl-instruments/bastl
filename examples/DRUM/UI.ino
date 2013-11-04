unsigned char page;
unsigned char sound,lastSound;

#define NUMBER_OF_PAGES 2
#define KNOB_MODE SMALL_BUTTON_2
#define DEFAULT_VELOCITY 127
boolean combo;
int var[NUMBER_OF_VOICES][6]; //??

boolean shift;
unsigned char volume[NUMBER_OF_VOICES];
unsigned char crush[NUMBER_OF_VOICES];
unsigned char currentSound[NUMBER_OF_VOICES];


void UI(){
  if(test) testMode();
  else{
    hw.setColor(page+2);
    renderSmallButtons();
    renderCombo();
    renderBigButtons();
    renderKnobs();
    renderTweaking(page);
    renderTimeStretch();
  }

}


#define VOLUME_COMPENSATION 100
void playSound(unsigned char _SOUND, unsigned char _VELOCITY){
  unsigned char voice=_SOUND%3;
  currentSound[voice]=_SOUND;
  unsigned char _sample=map(getVar(_SOUND,SAMPLE),0,8,0,NUMBER_OF_WAVETABLES);
  aSample[voice].setTable(WAVE_TABLES[_sample],WAVE_TABLES_NUM_CELLS[_sample]);
  aSample[voice].setFreq((float)(getVar(_SOUND,RATE)+1)/16);
  aSample[voice].setTimeStretch((getVar(_SOUND,STRETCH)<<2)+1);
  aSample[voice].setEnd(map(getVar(_SOUND,CUT),0,255,16,aSample[voice].getLength()));
  volume[voice]=(getVar(_SOUND,VOLUME)*(_VELOCITY))>>7;
  crush[voice]=getVar(_SOUND,CRUSH);

  aSample[voice].start();


}


int shiftSpeed;

void renderTimeStretch(){
  for(int voice=0;voice<NUMBER_OF_VOICES;voice++){
    if(aSample[voice].isPlaying()) aSample[voice].timeStretchUpdate(); 
  }
}

void renderTweaking(unsigned char _page){

  for(int voice=0;voice<NUMBER_OF_VOICES;voice++){

    switch(_page){
    case PAGE_G:
      aSample[voice].setFreq((float)(getVar(currentSound[voice],RATE)+1)/16);
      aSample[voice].setTimeStretch((getVar(currentSound[voice],STRETCH)<<2)+1);
      aSample[voice].setEnd(map(getVar(currentSound[voice],CUT),0,255,16,aSample[voice].getLength()));
      break;

    case PAGE_B:
      volume[voice]=getVar(currentSound[voice],VOLUME);
      crush[voice]=getVar(currentSound[voice],CRUSH);
      break;
    }


  }
  //tweaking end 
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
void renderCombo(){

  //loading saving presets
  // if(hw.buttonState(SMALL_BUTTON_2) && hw.justPressed(SMALL_BUTTON_1)) loadPreset(0), hw.freezeAllKnobs(), combo=true;
  for(int i=0;i<3;i++){
    if(hw.buttonState(EXTRA_BUTTON_2) && hw.justPressed(i)) randomize(i+3*shift); // randomize 
    if(hw.buttonState(SMALL_BUTTON_1) && hw.buttonState(SMALL_BUTTON_2) && hw.justPressed(i)) storePreset(currentPreset), loadPreset(i+3*hw.buttonState(EXTRA_BUTTON_2)), hw.freezeAllKnobs(), combo=true; // save&load
    if(hw.buttonState(SMALL_BUTTON_2) && hw.justPressed(i)) loadPreset(i+3*hw.buttonState(EXTRA_BUTTON_2)), hw.freezeAllKnobs(), combo=true; // load
  }

  if(combo){
    //turn off combo when all buttons are released 
    unsigned char _count=0; 
    for(int i=0;i<NUMBER_OF_BUTTONS;i++)  _count+=hw.buttonState(i); // if(!hw.buttonState(i)) combo=false;
    if(_count==0) combo=false;
    //else combo=true;
  } 


}

void randomize(unsigned char _sound){
  for(int i=0;i<NUMBER_OF_VARIABLES;i++) setVar(_sound,i,rand(maxValue[i])), hw.freezeAllKnobs();
}
void renderBigButtons(){

  if(!combo){
    for(int i=BIG_BUTTON_1;i<=BIG_BUTTON_3;i++) {
      hw.setLed(i,hw.buttonState(i));
      if(hw.justPressed(i)) {
        if(!shift) sound=i, playSound(sound,DEFAULT_VELOCITY), shiftSpeed=0;
        else sound=i+3, playSound(sound,DEFAULT_VELOCITY);
      }
      if(hw.justReleased(i)) ;
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
        if(inBetween( scale(hw.knobValue(i),KNOB_BITS,variableDepth[_variable]), scale(hw.lastKnobValue(i),KNOB_BITS,variableDepth[_variable]),getVar(sound,_variable) ) ) hw.unfreezeKnob(i),hw.setColor(WHITE); //external unfreez
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
































