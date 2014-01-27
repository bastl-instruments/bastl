unsigned char page;
unsigned char sound,lastSound;
boolean metronome;
#define NUMBER_OF_PAGES 2
#define KNOB_MODE SMALL_BUTTON_2
#define DEFAULT_VELOCITY 127
boolean combo;
//int var[NUMBER_OF_VOICES][6]; //??

boolean shift;
unsigned char volume[NUMBER_OF_VOICES];
unsigned char crush[NUMBER_OF_VOICES];
unsigned char currentSound[NUMBER_OF_VOICES];

boolean slave,record,erease;
boolean jump;
unsigned char jumpStep;
unsigned char currentPattern=0,lastStep;
#define CLOCK_BORDER 2
boolean lastClockCount;
int globalPitch;
unsigned char globalStretch, globalCut, globalCrush,globalVolume;
unsigned char mysteryShuffle;
boolean global;

void UI(){

  if(test) testMode();
  else{
    lastClockCount=seq.clockCount();
    renderSequencer();

    if(!seq.isPlaying()) hw.setColor(page+2);
    renderSmallButtons();
    renderCombo();
    renderBigButtons();
    renderKnobs();
    renderTweaking(page);
    renderTimeStretch();

  }

}


#define VOLUME_COMPENSATION 100
boolean hardMystery;
boolean doStretch[NUMBER_OF_VOICES];
void playSound(unsigned char _SOUND, unsigned char _VELOCITY){

  unsigned char voice; //=_SOUND;
  if(_SOUND==5) voice=2;
  else voice=_SOUND;
  // unsigned char voice=_SOUND%3;
  currentSound[voice]=_SOUND;


  if(dealMystery(mysteryShuffle,seq.getCurrentStep())){
    if(hardMystery){
      unsigned char _sample=rand(NUMBER_OF_WAVETABLES);
      aSample[voice].setTable(WAVE_TABLES[_sample],WAVE_TABLES_NUM_CELLS[_sample]);
      aSample[voice].setFreq((float)(rand(1024))/16);
    }
    else{
      aSample[voice].setTimeStretch(rand(255)+1);
      aSample[voice].setEnd(map(rand(255),0,255,16,aSample[voice].getLength()));
      volume[voice]=(rand(255)*(_VELOCITY))>>7;
      crush[voice]=rand(255);
    } 
  }

  else{

    unsigned char _sample=map(getVar(_SOUND,SAMPLE),0,8,0,NUMBER_OF_WAVETABLES);
    aSample[voice].setTable(WAVE_TABLES[_sample],WAVE_TABLES_NUM_CELLS[_sample]);
    aSample[voice].setFreq((float)(getVar(_SOUND,RATE)+1)/16);
    int _stretch=(getVar(_SOUND,STRETCH));
    if(_stretch==0) doStretch[voice]=false;
    else doStretch[voice]=true;
    aSample[voice].setTimeStretch((256-_stretch)<<2);
    aSample[voice].setEnd(map(getVar(_SOUND,CUT),0,255,16,aSample[voice].getLength()));
    volume[voice]=(getVar(_SOUND,VOLUME)*(_VELOCITY))>>7;
    crush[voice]=getVar(_SOUND,CRUSH);
  }


  aSample[voice].start();


}


int shiftSpeed;
unsigned char counter;
void renderTimeStretch(){
  if(counter++ > 2) counter=0;
  if(counter==0){
    for(int voice=0;voice<NUMBER_OF_VOICES-1;voice++){
      if(aSample[voice].isPlaying() && doStretch[voice]) aSample[voice].timeStretchUpdate(); 
    }
  }
}

void renderTweaking(unsigned char _page){

  for(int voice=0;voice<NUMBER_OF_VOICES-1;voice++){
    if(aSample[voice].isPlaying()){
      switch(_page){
      case PAGE_G:
        if(global && seq.isPlaying()){
          if(!hw.knobFreezed(TOP_KNOB)) aSample[voice].setFreq((float)(globalPitch+1)/16);
          if(!hw.knobFreezed(LEFT_KNOB)){

            int _stretch=globalStretch;
            if(_stretch==0) doStretch[voice]=false;
            else doStretch[voice]=true;
            aSample[voice].setTimeStretch(256-_stretch<<2);
          }

          if(!hw.knobFreezed(RIGHT_KNOB)) aSample[voice].setEnd(map(globalCut,0,255,16,aSample[voice].getLength()));
        }
        else{
          if(!hw.knobFreezed(TOP_KNOB)) aSample[voice].setFreq((float)(getVar(currentSound[voice],RATE)+1)/16);
          if(!hw.knobFreezed(LEFT_KNOB)){

            int _stretch=getVar(currentSound[voice],STRETCH);
            if(_stretch==0) doStretch[voice]=false;
            else doStretch[voice]=true;
            aSample[voice].setTimeStretch((256-_stretch)<<2);
          }
          
          //if(!hw.knobFreezed(LEFT_KNOB)) aSample[voice].setTimeStretch((getVar(currentSound[voice],STRETCH))+1);
          if(!hw.knobFreezed(RIGHT_KNOB)) aSample[voice].setEnd(map(getVar(currentSound[voice],CUT),0,255,16,aSample[voice].getLength()));
        }
        break;

      case PAGE_B:
        if(global&& seq.isPlaying()){    
          if(!hw.knobFreezed(RIGHT_KNOB)) volume[voice]=globalVolume;
          if(!hw.knobFreezed(LEFT_KNOB))  crush[voice]=globalCrush;
        }
        else{     
          if(!hw.knobFreezed(RIGHT_KNOB)) volume[voice]=getVar(currentSound[voice],VOLUME);
          if(!hw.knobFreezed(LEFT_KNOB))  crush[voice]=getVar(currentSound[voice],CRUSH);
        }
        break;

      }

    }
  }
  //tweaking end 
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
  shift=hw.buttonState(SHIFT);
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
    if(!seq.isPlaying() && hw.buttonState(EREASE) && hw.justPressed(i)){
      sound=i+3*shift;
      randomize(sound);
      playSound(sound,DEFAULT_VELOCITY);
      combo=true;
      hw.setLed(i,true);
    }

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
    else if(!slave) MIDI.sendRealTime(Stop),record=false;//,global=false;//, sendAllNoteOff();
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





  if(combo){
    //turn off combo when all buttons are released 
    unsigned char _count=0; 
    for(int i=0;i<NUMBER_OF_BUTTONS;i++)  _count+=hw.buttonState(i); 
    if(_count==0) combo=false,countLong=false, longCount=0,longPress=false;
  } 


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
  setVar(_sound,SAMPLE,rand(NUMBER_OF_WAVETABLES));
  setVar(_sound,VOLUME,rand(50)+200);
}

void randomizePattern(unsigned char _pattern){
  for(int i=0;i<NUMBER_OF_PATTERN_BYTES;i++) pattern[_pattern][i]=rand(100);

}
void renderBigButtons(){

  if(!combo){
    for(int i=BIG_BUTTON_1;i<=BIG_BUTTON_3;i++) {
      if(global&& seq.isPlaying()){

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

      else{


        if(hw.buttonState(i)) hw.setLed(i,true);
        if(!seq.isPlaying() && !hw.buttonState(i)) hw.setLed(i,false);
        if(longPress) hw.setLed(i,true);


        if(seq.isPlaying() && erease){

          if(lastClockCount!=seq.clockCount()){
            if(hw.buttonState(i)){
              if(seq.clockCount()>CLOCK_BORDER){
                if(seq.getCurrentStep()==(seq.getNumberOfSteps()-1)) writeToStep(currentPattern,0,shift*3+i,false);
                else writeToStep(currentPattern,seq.getCurrentStep()+1,shift*3+i,false);
              }
              else writeToStep(currentPattern,seq.getCurrentStep(),shift*3+i,false) ;
            } 
          }
        }

        if(record){
          if(hw.justPressed(i)){
            if(seq.clockCount()>CLOCK_BORDER){
              if(seq.getCurrentStep()==(seq.getNumberOfSteps()-1)) writeToStep(currentPattern,0,shift*3+i,true);
              else writeToStep(currentPattern,seq.getCurrentStep()+1,shift*3+i,true);
            }
            else writeToStep(currentPattern,seq.getCurrentStep(),shift*3+i,true) ;//MIDI.sendNoteOn(3*shift+i,DEFAULT_VELOCITY,outChannel[page]);

            if(!shift) sound=i, playSound(sound,DEFAULT_VELOCITY), shiftSpeed=0;
            else sound=i+3, playSound(sound,DEFAULT_VELOCITY);
          }
        }
        else{
          if(hw.justPressed(i)) {

            // sound=i+3*shift;

            if(!shift) sound=i, playSound(sound,DEFAULT_VELOCITY), shiftSpeed=0;
            else sound=i+3, playSound(sound,DEFAULT_VELOCITY);

          }
          if(sound!=lastSound)  hw.freezeAllKnobs();
          lastSound=sound;
          for(int i=BIG_BUTTON_1;i<=BIG_BUTTON_3;i++){
            if(longPress) hw.setLed(i,true);
          }

        }
      }
    } 
  }
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
}

void renderKnobs(){

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
    if(page==0){

      if(hw.knobFreezed(KNOB_TOP)) {
        if(hw.knobMoved(KNOB_TOP)) hw.unfreezeKnob(KNOB_TOP); //external unfreez
      }
      else globalPitch=hw.knobValue(KNOB_TOP);

      if(hw.knobFreezed(KNOB_LEFT)) {
        if( hw.knobMoved(KNOB_LEFT))  hw.unfreezeKnob(KNOB_LEFT); //external unfreez
      }
      else globalStretch=hw.knobValue(KNOB_LEFT)>>2;  

      if(hw.knobFreezed(KNOB_RIGHT)) {
        if(hw.knobMoved(KNOB_RIGHT))  hw.unfreezeKnob(KNOB_RIGHT); //external unfreez
      }
      else globalCut=hw.knobValue(KNOB_RIGHT)>>2;

    }
    else if(page==1){

      if(hw.knobFreezed(KNOB_TOP)) {
        if(hw.knobMoved(KNOB_TOP)) hw.unfreezeKnob(KNOB_TOP); //external unfreez
      }
      else mysteryShuffle=hw.knobValue(KNOB_TOP)>>6;

      if(hw.knobFreezed(KNOB_LEFT)) {
        if(hw.knobMoved(KNOB_LEFT))  hw.unfreezeKnob(KNOB_LEFT); //external unfreez
      }
      else globalCrush=hw.knobValue(KNOB_LEFT)>>2;  

      if(hw.knobFreezed(KNOB_RIGHT)) {
        if(hw.knobMoved(KNOB_RIGHT))  hw.unfreezeKnob(KNOB_RIGHT); //external unfreez
      }
      else globalVolume=hw.knobValue(KNOB_RIGHT)>>2;

    }
  }
  else {
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

































