
unsigned char counter;
#define COUNTER_LIMIT 32
#define CC_OFFSET 10
#define TEST_CHANNEL 1
#define CC_SHIFT 3
#define BOOLEAN_RANDOM 2
#define PAGE_RGB_OVERFLOW 2
#define COUNTER_RESET 0
#define PAGE2COLOR 1
#define ANY_SOUND 1
boolean sendSwitch;

int counter2;
void testMode(){
  counter2++;
  MIDI.read(TEST_CHANNEL);
  if(counter2>8){
    counter++;
    if(counter>COUNTER_LIMIT) counter=COUNTER_RESET,anySound();

  }
  if(hw.justPressed(SMALL_BUTTON_1)) page=increaseValue(page,PAGE_RGB_OVERFLOW) , MIDI.sendControlChange(PAGE_BYTE,page,TEST_CHANNEL);
  if(hw.justPressed(SMALL_BUTTON_2)) page=decreaseValue(page,PAGE_RGB_OVERFLOW), MIDI.sendControlChange(PAGE_BYTE,page,TEST_CHANNEL);
  hw.setColor(page+1);


  for(int i=0;i<NUMBER_OF_KNOBS;i++){
    if(hw.justPressed(i))  hw.flipSwitch(i), sendSwitch=true;
    hw.setLed(i,hw.switchState(i));
    hw.unfreezeKnob(i);

    
 if(counter2>8){ 
      
      if(hw.knobMoved(i)) MIDI.sendControlChange(CC_OFFSET+i,hw.knobValue(i)>>CC_SHIFT,TEST_CHANNEL),  hw.setColor(WHITE);
    }
    if(hw.buttonState(EXTRA_BUTTON_1) || hw.buttonState(EXTRA_BUTTON_2) ) hw.setSwitch(i,random(BOOLEAN_RANDOM));

  }
  if(sendSwitch) MIDI.sendControlChange(SWITCH_BYTE,hw.soundFromSwitches(),TEST_CHANNEL), sendSwitch=false;


  if(hw.buttonState(EXTRA_BUTTON_1) || hw.buttonState(EXTRA_BUTTON_2) ){
    for(int i=0;i<NUMBER_OF_KNOBS;i++)  hw.setSwitch(i,random(BOOLEAN_RANDOM));
  }

if(counter2>8) counter2=0;

}


void anySound(){
  aSample.setFreq(50), aSample.start();
}
void stopSound(){

}



int decreaseValue(int _VALUE, int _OVERFLOW){
  if(_VALUE > 0) _VALUE--;
  else _VALUE=_OVERFLOW;
  return _VALUE;
}







