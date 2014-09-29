boolean combo=false;
unsigned char page=0;
unsigned char volume[3];
#define NUMBER_OF_PAGES 3
boolean shift, lastShift;
//unsigned char lastSound;

unsigned char syncSig[3]; //2bit
unsigned char syncOsc[3]; //2bit
//WT1 2bit
//WT2 2bit
unsigned char rgbPins[3]={
  5,3,4};
boolean ledShow;
void UI(){
  lastShift=shift;
  shift=hw.buttonState(SMALL_BUTTON_1);
  if(lastShift!=shift) hw.freezeAllKnobs();

  if(hw.justReleased(SMALL_BUTTON_2) && !combo){ 
    if(page<NUMBER_OF_PAGES-1) page++;
    else page=0;
    hw.freezeAllKnobs();
  } 

  for(int i=0;i<3;i++){
    if(hw.justPressed(i)){
      hw.flipSwitch(i);
      sound=hw.soundFromSwitches();
      hw.freezeAllKnobs();


    }
    hw.setLed(i,hw.switchState(i));
  }
  if(hw.justPressed(EXTRA_BUTTON_2)) randomize(sound), hw.freezeAllKnobs();
  renderKnobs();
  renderTweaking();
  if(hw.buttonState(SMALL_BUTTON_2)){
    ledShow=false;
    for(int i=0;i<3;i++){
      if((page+1)%3==i) PORTD &= ~(1<<rgbPins[i]);
      else PORTD |= 1<<rgbPins[i];
    }

  }
  else ledShow=true;
}
int var[8][3][6];
void renderKnobs(){
  hw.setColor(page+1);
  for(int i=0;i<NUMBER_OF_KNOBS;i++){

    // unsigned char _variable=i+VARIABLES_PER_PAGE*page;


    if(hw.knobFreezed(i)) {
      //   if(lastSound==sound){ // knobs are unfreezed when they come across their original value
      if(hw.knobMoved(i)){
        hw.unfreezeKnob(i); //external unfreez
        hw.setColor(WHITE);
      }
      //}  
    }

    else var[sound][page][i+3*shift]=hw.knobValue(i);//setVar(sound,_variable,scale(hw.knobValue(i),KNOB_BITS,variableDepth[_variable])); 

  }
}

//unsigned char syncSig[3]; //2bit
//unsigned char syncOsc[3];
float frequencyA[3];
float frequencyB[3];
void renderTweaking(){
  for(int i=0;i<3;i++){

    frequencyA[i]=float(var[sound][i][1]*2+float((float)var[sound][i][4]/50.0));
    aOsc[i].setFreq(frequencyA[i]);
    frequencyB[i]=float(float((float)var[sound][i][2]/10.0)+float((float)var[sound][i][5]/500.0));
    aOsc[3+i].setFreq(frequencyB[i]);
    volume[i]=var[sound][i][0]>>2;
    syncSig[i]=getBits(var[sound][i][3],4,2);
    syncOsc[i]=getBits(var[sound][i][3],6,2);
    aOsc[i].setTable(WAVE_TABLES[getBits(var[sound][i][3],0,2)]);
    aOsc[i+3].setTable(WAVE_TABLES[getBits(var[sound][i][3],2,2)]);
  }


}



unsigned char getBits(unsigned char _val, unsigned char _offset, unsigned char _bits){
  unsigned char returnVal=0;
  for(int i=0;i<_bits;i++){
    bitWrite(returnVal,i,bitRead(_val,_offset+i));
  }
  return  returnVal;
}

void randomize(unsigned char _sound){
  for(int i=0;i<3;i++){
    for(int j=0;j<6;j++){
      var[_sound][i][j]=random(1023);
    }
  } 

}




