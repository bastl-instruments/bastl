
PROGMEM prog_uchar clearTo[]={
  0,0,0,0, 64,0,31,0, 0,127,0,0};
/*

wavetable
osc page 
arp rate xor amt osc char (xor+wt) -  B arpTYPE
lfo page
lfo rate, lfo res, lfo amt, B-lfo shape
adsr page
atck,sus, rel B - decay
 
 */

// variable names
//page 0


#define OSC_PAGE 0
#define LFO_PAGE 1
#define ADSR_PAGE 2

#define VARIABLES_PER_PAGE 4

#define ARP_RATE 0 // 127 -7
#define XOR_AMT 1 // 127 -7
#define OSC_CHAR 2 // WT + XOR 3*3 = 9 - 4 bity
#define ARP_TYPE 3 // 5 -4

//7+7+4+4 = 22

#define LFO_RATE 4 // 127 - 7
#define LFO_AMT 5 // 127 - 7
#define LFO_RES 6 // 31 -5
#define LFO_SHAPE 7 // 7 - 3
//7+7+5+3 = 22


#define ATACK 8 // 127 - 7
#define SUSTAIN 9 // 127 - 7
#define RELEASE 10 // 127 -7
#define DECAY 11 // 7 - 3

// 7+7+7+3 = 24

//22+22+24 = 68

// =>68 bitu => 9 bytu

//8 zvuku *9 bytů = 72 bytů / preset

#define KNOB_BITS 10


PROGMEM prog_uint16_t maxValue[]={
  127,127,15,15,  127,127,31,7, 127,127,127,7}; //const
  

#define NUMBER_OF_VARIABLES 12
#define NUMBER_OF_BYTES 9
#define NUMBER_OF_PRESETS 6

//8*9
#define PRESET_SIZE 72

#define NUMBER_OF_SOUNDS 8


//?
#define ADSR_BITS 7
#define SUSTAIN_BITS 8

#define CHANNEL_BYTE 1023

unsigned char variableDepth[NUMBER_OF_VARIABLES];//={1,2,3,6,4,8,2,3,1,5,3,7,8,6,7};



int maxVal(int _maxVal){
  return  pgm_read_word_near(maxValue+_maxVal);
}

unsigned char byteCoordinate[NUMBER_OF_VARIABLES];
unsigned char bitCoordinate[NUMBER_OF_VARIABLES];
unsigned char currentPreset;
/*
int buffer[NUMBER_OF_SOUNDS][NUMBER_OF_VARIABLES];
 unsigned char bufferP[NUMBER_OF_STEPS];
 */

unsigned char variable[NUMBER_OF_SOUNDS][NUMBER_OF_BYTES];

void initMem(){

  calculateBitDepth();
  ShouldIClearMemory();
  if(hw.factoryClear()) clearMemmory(), hw.factoryCleared();
  hw.initialize(DEFAULT);
  loadPreset(0);

}

void calculateBitDepth(){
  for(int i=0;i<NUMBER_OF_VARIABLES;i++){ // calculate bitDepth according to the maximum value
    int x=0;
    while(maxVal(i)-pow(2,x)>=0) x++;
    variableDepth[i]=x ;
  }

  for(int i=0;i<NUMBER_OF_VARIABLES;i++){
    int sum=0;
    for(int j=0;j<i;j++){
      sum+=variableDepth[j];
    }
    byteCoordinate[i]=sum/8;
    bitCoordinate[i]=sum%8;
  } 
}

int getVar(unsigned char _SOUND, unsigned char _VARIABLE){

  int _value=0;
  unsigned char byteShift=0;
  unsigned char _bitCoordinate=0;
  for(int i=0;i<variableDepth[_VARIABLE];i++){

    if((bitCoordinate[_VARIABLE]+i)>15){
      byteShift=2;
      _bitCoordinate = i-(16-bitCoordinate[_VARIABLE]);
    }
    else if((bitCoordinate[_VARIABLE]+i)>7){
      byteShift=1; 
      _bitCoordinate = i-(8-bitCoordinate[_VARIABLE]);//bitCount;
    }
    else {
      _bitCoordinate=bitCoordinate[_VARIABLE]+i;
    }

    boolean bitState=bitRead(variable[_SOUND][byteCoordinate[_VARIABLE]+byteShift],_bitCoordinate);
    bitWrite(_value,i,bitState);

  }
  return _value; 

}

void setVar(unsigned char _SOUND, unsigned char _VARIABLE, int _value){

  unsigned char byteShift=0;
  unsigned char _bitCoordinate;
  for(int i=0;i<variableDepth[_VARIABLE];i++){



    if((bitCoordinate[_VARIABLE]+i)>15){
      byteShift=2;
      _bitCoordinate = i-(16-bitCoordinate[_VARIABLE]);
    }
    else if((bitCoordinate[_VARIABLE]+i)>7){
      byteShift=1;
      _bitCoordinate = i-(8-bitCoordinate[_VARIABLE]);
    }
    else {
      _bitCoordinate=bitCoordinate[_VARIABLE]+i;
    }

    boolean bitState=bitRead(_value,i);
    bitWrite(variable[_SOUND][byteCoordinate[_VARIABLE]+byteShift],_bitCoordinate,bitState);
  }

}


void storePreset(unsigned char index) {

  int offset = index * PRESET_SIZE;
  for (int j = 0; j < NUMBER_OF_SOUNDS; j++) {
    for (int k = 0; k < NUMBER_OF_BYTES; k++) {
      EEPROM.write(offset + ((NUMBER_OF_BYTES * j) + k), variable[j][k]);
    }
  }

}

void loadPreset(unsigned char index) {

  currentPreset=index;
  int offset = index * PRESET_SIZE;

  for (int j = 0; j < NUMBER_OF_SOUNDS; j++) {
    for (int k = 0; k < NUMBER_OF_BYTES; k++) {
      variable[j][k]=EEPROM.read(offset + ((NUMBER_OF_BYTES * j) + k));
    }
  }
 
}



void clearMemmory(){
  for(int x=0;x<NUMBER_OF_PRESETS;x++){
    loadPreset(x);
    for(int i=0;i<NUMBER_OF_SOUNDS;i++){
      for(int j=0;j<NUMBER_OF_VARIABLES;j++){
        setVar(i,j, pgm_read_byte_near(clearTo+j));
      }
    }
    //  tempo=120;

    storePreset(x);

  }
}


void ShouldIClearMemory(){ 
  hw.update();
  hw.update();
  if(ConditionToClear()){
    IndicateClearing(true);
    clearMemmory();
    IndicateClearing(false);
    setMidiChannel(0);
  }

}

boolean ConditionToClear(){
  boolean _shouldClear=false;
  //if something or something else _shouldClear=true;
  if(hw.buttonState(BIG_BUTTON_1) && hw.buttonState(BIG_BUTTON_2) && hw.buttonState(BIG_BUTTON_3)) _shouldClear=true;
  else if(hw.buttonState(BIG_BUTTON_1)) setMidiChannel(1+9*hw.buttonState(SMALL_BUTTON_1));
  else if(hw.buttonState(BIG_BUTTON_2)) setMidiChannel(2+9*hw.buttonState(SMALL_BUTTON_1));
  else if(hw.buttonState(BIG_BUTTON_3)) setMidiChannel(3+9*hw.buttonState(SMALL_BUTTON_1));
  
  return _shouldClear;
}

void setMidiChannel(unsigned char _channel){
  EEPROM.write(CHANNEL_BYTE,_channel);
}

unsigned char getMidiChannel(){
 return EEPROM.read(CHANNEL_BYTE);
}

void IndicateClearing(boolean _start){

  for(int i=0;i<NUMBER_OF_COLORS;i++) hw.setColor(i), delay(200), hw.update();
  delay(200);
  hw.setColor(0);
  hw.setLed(LED_1,_start);
  hw.setLed(LED_2,_start);
  hw.setLed(LED_3,_start);
  hw.update();

}

boolean inBetween(int val1,int val2,int inBet){

  boolean retVal;

  if(val1>=val2){
    if(inBet<=val1 && inBet>=val2) retVal=true;
    else retVal=false;
  }
  else if(val1<val2){
    if(inBet>=val1 && inBet<=val2) retVal=true;
    else retVal=false;
  }
 
  return retVal;

}

int scale(int _value, unsigned char _originalBits, unsigned char _endBits){
  if(_originalBits >= _endBits) return _value >> (_originalBits - _endBits);
  else return _value << (_endBits - _originalBits);
}

