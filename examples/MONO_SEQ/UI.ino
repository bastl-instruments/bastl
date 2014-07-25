#define OSC3 0
#define FM 1
#define LPF 2
#define AUDU 3

#define AM 4
#define XOR 5
#define ONE_LINER 6
#define FM2 7


unsigned char page;
unsigned char sound,lastSound;
boolean metronome;
#define NUMBER_OF_PAGES 2
#define KNOB_MODE SMALL_BUTTON_2
#define DEFAULT_VELOCITY 127
boolean combo;
//int var[NUMBER_OF_VOICES][6]; //??
boolean renderM;

boolean shift;

/*
unsigned char volume[NUMBER_OF_VOICES];
 unsigned char crush[NUMBER_OF_VOICES];
 unsigned char currentSound[NUMBER_OF_VOICES];
 */

boolean slave,record,erease;
boolean jump;
unsigned char jumpStep;
unsigned char currentPattern=0;
#define CLOCK_BORDER 2
boolean lastClockCount;

unsigned char mysteryShuffle;
boolean global;
unsigned char globalVar[6];
boolean globalize[6];

uint16_t syncPhaseAcc;
uint16_t syncPhaseInc;
uint16_t grainPhaseAcc;
uint16_t grainPhaseInc;
uint16_t grainAmp;
uint8_t grainDecay;
uint16_t grain2PhaseAcc;
uint16_t grain2PhaseInc;
uint16_t grain2Amp;
uint8_t grain2Decay;


void UI(){

  if(test) testMode();
  else{

    if(!metronome || !seq.isPlaying()) renderM=false;



    if(!seq.isPlaying()) hw.setColor(page+2);


    renderSmallButtons();


    renderCombo();

    renderSequencer();

    renderBigButtons();
    lastClockCount=seq.clockCount();




    //  renderLeds();

    renderKnobs();


    renderSound();


    renderLfoAdsr();



  }

}


#define VOLUME_COMPENSATION 100
boolean hardMystery;
//boolean doStretch[NUMBER_OF_VOICES];

#define CC_OFFSET 10
unsigned char oscType;
void getTypes(unsigned char _SOUND){
  for(int i=0;i<3;i++){
    if(globalize[i+3]) var[i]=globalVar[i+3];
    else var[i]=getVar(_SOUND,i+3); 
  }
}

void playSound(unsigned char _SOUND, unsigned char _VELOCITY){
  sound=0;

  if(dealMystery(mysteryShuffle,seq.getCurrentStep())){
    for(int i=0;i<3;i++){
      if(hardMystery) globalVar[i+3]=rand(255), globalize[i+3]=true;
      else globalize[i+3]=false;
      for(int i=0;i<3;i++) globalVar[i]=rand(255), globalize[i]=true;
    }
  }
  else  for(int i=0;i<6;i++) globalize[i]=false;


  for(int i=0;i<3;i++) bitWrite(sound,i,bitRead(_SOUND,i));
  if(_SOUND==0){
    ADSR.noteOff();
    for(int i=0;i<6;i++) MIDI.sendControlChange(CC_OFFSET+i,0,1);
  }
  else{
     for(int i=0;i<6;i++) MIDI.sendControlChange(CC_OFFSET+i,variable[_SOUND-1][i]>>1,1);
    ADSR.noteOff();
    renderSound();
    ADSR.noteOn(DEFAULT_VELOCITY);
  }
 

}


//int shiftSpeed;
unsigned char counter;
/*
void renderTimeStretch(){
 if(counter++ > 2) counter=0;
 if(counter==0){
 for(int voice=0;voice<NUMBER_OF_VOICES-1;voice++){
 if(aSample[voice].isPlaying() && doStretch[voice]) aSample[voice].timeStretchUpdate(); 
 }
 }
 }
 */
unsigned char modAmt;
unsigned char lfoDest,lfoAmt,adsrType;

int getLfoNow(unsigned char _PARAM){
  return isDestination(_PARAM)*((LFO.next()*lfoAmt)>>8);

}
boolean isDestination(unsigned char _PARAM){
  if(lfoDest>=3) return true;
  else if(_PARAM==lfoDest) return true;
  else return false; 
}

void renderLfoAdsr(){
  LFO.update();
  ADSR.update();
}


#define SHIFT SMALL_BUTTON_1
#define PATTERN EXTRA_BUTTON_1
#define PAGE SMALL_BUTTON_2
#define EREASE EXTRA_BUTTON_2

void renderSmallButtons(){
  // knobMode
  if(hw.justReleased(PAGE) && !combo){ 

    page=increaseValue(page,NUMBER_OF_PAGES-1);
    hw.freezeAllKnobs();
  } 
  if(hw.justPressed(SHIFT)) shift=!shift, hw.resetSwitches();
  //  shift=hw.switchState(SHIFT);
  erease=hw.buttonState(EREASE);


}

#define LONG_THRESHOLD 120
boolean countLong, longPress;
int longCount;





/*
   DRUMseq
 
 page > bigBut = loadPreset
 page + shift > bigBut = savePreset
 
 page > erease = play/stop
 erease > page = record
 
 shift > page = tap
 shift > erease = metronome
 
 
 
 
 erease > bigButton = randomizeSound
 
 PTRN > page = ?global page
 on green bigBut changes pattern 
 on blue page bigBut is jump
 
 PTRN > erease = pattern clear
 PTRN > bigButt(shift) = change pattern
 patern 6 is alway a new random pattern (it is possible to copy/paste)
 
 PTRN + shift > erease = copy pattern
 PTRN + shift > page = paste pattern
 
 
 (PTRN) (shift) 				(page) (EREASE)
 		[big1] [big2] [big3] 
 */

void renderCombo(){

  if(!seq.isPlaying() && hw.justReleased(EREASE) && sound>0 && !combo ) randomize(sound-1), playSound(sound,DEFAULT_VELOCITY);

  if(countLong) {
    longCount++;
    if(longCount>LONG_THRESHOLD) longPress=true;
  }


  for(int i=0;i<3;i++){
    //       page + shift > bigBut = savePreset

    if(hw.buttonState(SHIFT) && hw.buttonState(PAGE) && hw.justPressed(i)) countLong=true,longCount=0, combo=true; // save&load
    if(hw.buttonState(SHIFT) && hw.buttonState(PAGE) && hw.justReleased(i)){
      if(longPress) storePreset(currentPreset),loadPreset(i+3), hw.freezeAllKnobs(), combo=true; // save&load
      else storePreset(currentPreset), loadPreset(i), hw.freezeAllKnobs(), combo=true;
    }
    //page > bigBut = loadPreset
    if(hw.buttonState(PAGE) && hw.justPressed(i)) countLong=true,longCount=0, combo=true; // load
    if(hw.buttonState(PAGE) && hw.justReleased(i)){
      if(longPress) loadPreset(i+3), hw.freezeAllKnobs();
      else loadPreset(i), hw.freezeAllKnobs();
    }
    if(longPress) hw.setLed(i,true);

    // erease > bigButton = randomizeSound


    //   PTRN > bigButt(shift) = change pattern
    if(hw.buttonState(PATTERN) && hw.justPressed(i)){
      currentPattern=i+shift*3, combo=true;
      if(currentPattern==5) randomizePattern(currentPattern);
    }

    //  patern 6 is alway a new random pattern (it is possible to copy/paste)

  }




  // page > erease = play/stop
  if(hw.buttonState(PAGE) && hw.justPressed(EREASE)){
    seq.playStop(), combo=true; 
    if(seq.isPlaying() && !slave) MIDI.sendRealTime(Start);
    else if(!slave) MIDI.sendRealTime(Stop),record=false,sound=0;//,playSound(0,DEFAULT_VELOCITY);//,global=false;//, sendAllNoteOff();
    hw.freezeAllKnobs();
  }

  //erease > page = record
  if(hw.buttonState(EREASE) && hw.justPressed(PAGE)) hw.freezeAllKnobs(),  record=!record, combo=true; // seq.start();
  if(record) global=false;





  // PTRN + shift > erease = copy pattern
  if(hw.buttonState(PATTERN) && hw.buttonState(SHIFT) && hw.justPressed(EREASE)) copyPattern(currentPattern), combo=true;

  // PTRN > erease = pattern clear
  else if(hw.buttonState(PATTERN) && hw.justPressed(EREASE)) clearPattern(currentPattern), combo=true;

  //  shift > erease = metronome
  else if(hw.buttonState(SHIFT) && hw.justPressed(EREASE))  metronome=!metronome,combo=true;


  //  PTRN + shift > page = paste pattern
  if(hw.buttonState(PATTERN) && hw.buttonState(SHIFT) && hw.justPressed(PAGE)) pastePattern(currentPattern), combo=true;

  // PTRN > page = ?global page
  else if(hw.buttonState(PATTERN) && hw.justPressed(PAGE)) global=!global, combo=true, hw.freezeAllKnobs();
  // on green bigBut changes pattern 
  //on blue page bigBut is jump

  // shift > page = tap
  else if(hw.buttonState(SHIFT) && hw.justPressed(PAGE)) seq.tapIn(audioTicks()), combo=true;

  //pokemon
  if(hw.buttonState(SHIFT) && hw.buttonState(PATTERN) && hw.buttonState(PAGE) && hw.buttonState(EREASE)) combo=true,makeSysExArray(sound), sendSysExArray();


  if(combo){
    //turn off combo when all buttons are released 
    unsigned char _count=0; 
    for(int i=0;i<NUMBER_OF_BUTTONS;i++)  _count+=hw.buttonState(i); 
    if(_count==0) combo=false,countLong=false, longCount=0,longPress=false;
  } 
  // 

}

unsigned char copyMemory[NUMBER_OF_PATTERN_BYTES];
void copyPattern(unsigned char _pattern){
  for(int i=0;i<NUMBER_OF_PATTERN_BYTES;i++){
    copyMemory[i]=pattern[_pattern][i]; 
  }
}

void pastePattern(unsigned char _pattern){
  for(int i=0;i<NUMBER_OF_PATTERN_BYTES;i++){
    pattern[_pattern][i]=copyMemory[i];
  }
}
void randomize(unsigned char _sound){
  for(int i=0;i<NUMBER_OF_VARIABLES;i++) setVar(_sound,i,rand(maxVal(i))),hw.freezeAllKnobs();
  //  setVar(_sound,SAMPLE,rand(NUMBER_OF_WAVETABLES));
  // setVar(_sound,VOLUME,rand(50)+200);
}

void randomizePattern(unsigned char _pattern){
  for(int i=0;i<NUMBER_OF_PATTERN_BYTES;i++) pattern[_pattern][i]=rand(100);

}
void renderBigButtons(){

  if(!combo){
    if(global && seq.isPlaying()){
      for(int i=BIG_BUTTON_1;i<=BIG_BUTTON_3;i++) {
        if(page==0){
          if(i==currentPattern || i==currentPattern-3) hw.setLed(i,true);
          else hw.setLed(i,false);
          if(hw.justPressed(i)) currentPattern=i;
          if(erease && hw.justPressed(i)) clearPattern(i);
        }
        else if(page==1){
          if(hw.buttonState(i)){     
            jump=true;
            jumpStep=(i*4+shift*16);
            //  combo=true;
          }
          // else jump=false;
        }
      }
    }

    else{
      if(seq.isPlaying() && erease){
        if(lastClockCount!=seq.clockCount()){
          sound=0;
          // if(hw.buttonState(i)){
          if(seq.clockCount()>CLOCK_BORDER){
            if(seq.getCurrentStep()==(seq.getNumberOfSteps()-1)) setStep(currentPattern,0,0);
            else setStep(currentPattern,seq.getCurrentStep()+1,0);
          }
          else setStep(currentPattern,seq.getCurrentStep(),0) ;
          //} 
        }
        sound=0;
        if(soundChanged()) playSound(sound,DEFAULT_VELOCITY);
      }

      else if(record){

        if(lastClockCount!=seq.clockCount()){
          sound=soundFromButtons();

          if(seq.clockCount()>CLOCK_BORDER){
            if(seq.getCurrentStep()==(seq.getNumberOfSteps()-1))  sound|=getStep(currentPattern,0), setStep(currentPattern,0,sound); //sound|=getStep(currentPattern,0),
            else sound|=getStep(currentPattern,seq.getCurrentStep()+1), setStep(currentPattern,seq.getCurrentStep()+1,sound); //
          }
          else sound|=getStep(currentPattern,seq.getCurrentStep()), setStep(currentPattern,seq.getCurrentStep(),sound) ;// MIDI.sendNoteOn(3*shift+i,DEFAULT_VELOCITY,outChannel[page]);
          if(soundChanged()) playSound(sound,DEFAULT_VELOCITY);
        }
      }
      else if(!seq.isPlaying()){
        sound=soundFromButtons();
        if(soundChanged()) hw.freezeAllKnobs(),playSound(sound,DEFAULT_VELOCITY);
      }
    } 
  }
  renderLeds();
  if(hw.buttonState(PATTERN)){
    for(int i=0;i<3;i++){
      hw.setLed(i,false);
      if(shift) {
        if(currentPattern==(i+3)) hw.setLed(i,true);
      }
      else{
        if(currentPattern==i) hw.setLed(i,true);
      }
    }
  }
  lastSound=sound;

}
unsigned char soundFromButtons(){
  if(shift && !seq.isPlaying()) {
    for(int i=0;i<3;i++){
      if(hw.justPressed(i)) hw.flipSwitch(i); 
    }
    return hw.soundFromSwitches();
  }
  else{
    /*
    for(int i=0;i<3;i++){
     if(hw.justPressed(i)) butState[i]=true;
     
     if(hw.justReleased(i)) released[i]=true, releaseCount[i]++;
     if(released[i] && (releaseCount[i] > releaseThreshold)) butState[i]=false;
     }
     */
    hw.resetSwitches();
    return hw.soundFromButtons(); 
  }

}
void renderLeds(){
  for(int i=BIG_BUTTON_1;i<=BIG_BUTTON_3;i++) {

    hw.setLed(i,bitRead(sound,i));

    //        if(!seq.isPlaying() && !hw.buttonState(i)) hw.setLed(i,false);
    if(longPress) hw.setLed(i,true);
  } 
}
boolean soundChanged(){
  /*
  for(int i=0;i<3;i++){
   if(hw.justPressed(i) || hw.justReleased(i)){
   if(shift){
   if(hw.justPressed(i)){
   hw.flipSwitch(i);
   sound=hw.soundFromSwitches(); 
   }
   }
   else sound=hw.soundFromButtons();
   } 
   }
   */
  if(sound!=lastSound) return true;
  else return false;
}

void renderKnobs(){
  unsigned char _SOUND;
 // if(seq.isPlaying()){
    if(hw.soundFromButtons()>0) _SOUND=hw.soundFromButtons()-1;//sound-1;
 // }
 // else _SOUND
  mysteryShuffle=0;
  if(record){

    if(hw.knobFreezed(KNOB_TOP)) {
      if(inBetween(20+hw.knobValue(KNOB_TOP)>>2,20+hw.lastKnobValue(KNOB_TOP)>>2,seq.getTempo()  )) hw.unfreezeKnob(KNOB_TOP); //external unfreez
    }
    else seq.setTempo(20+hw.knobValue(KNOB_TOP)>>2);    

    if(hw.knobFreezed(KNOB_LEFT)) {
      if(inBetween(1+hw.knobValue(KNOB_LEFT)>>5,1+hw.lastKnobValue(KNOB_LEFT)>>5,seq.getNumberOfSteps()  )) hw.unfreezeKnob(KNOB_LEFT); //external unfreez
    }
    else seq.setNumberOfSteps(1+hw.knobValue(KNOB_LEFT)>>5);   

    if(hw.knobFreezed(KNOB_RIGHT)) {
      if(inBetween(hw.knobValue(KNOB_RIGHT)>>2,hw.lastKnobValue(KNOB_RIGHT)>>2,seq.getGrooveAmount()  )) hw.unfreezeKnob(KNOB_RIGHT); //external unfreez
    }
    else seq.setGrooveAmount(hw.knobValue(KNOB_RIGHT)>>2); 

  }

  else if(global && seq.isPlaying()){
    for(int i=0;i<NUMBER_OF_KNOBS;i++){
      if(hw.knobFreezed(i)) {
        globalize[i+page*3]=false;
        if(hw.knobMoved(i)) hw.unfreezeKnob(i); //external unfreez

      }
      else globalVar[i+page*3]=hw.knobValue(i)>>2, globalize[i+page*3]=true;
    }
  }

  else if(sound!=0){
    for(int i=0;i<NUMBER_OF_KNOBS;i++){
      unsigned char _variable=i+VARIABLES_PER_PAGE*page;

      if(hw.knobFreezed(i)) {
        if(lastSound==sound){ // knobs are unfreezed when they come across their original value
          if(inBetween( hw.knobValue(i)>>2,hw.lastKnobValue(i)>>2,getVar(_SOUND,_variable) ) ) hw.unfreezeKnob(i),hw.setColor(WHITE); //external unfreez
        }  
      }

      else{       
        unsigned char _var=getVar(_SOUND,_variable);
        unsigned char _knobVar=hw.knobValue(i)>>2;
        if((_knobVar>>1)!=(_var>>1)) MIDI.sendControlChange(CC_OFFSET+_variable,_knobVar>>1,1);
        setVar(_SOUND,_variable,_knobVar); 

      }
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

void clearPattern(unsigned char _pattern){
  for(int i=0;i<32;i++){
    for(int j=0;j<6;j++){
      writeToStep(_pattern,i,j,false);
    }
  }
}






































































