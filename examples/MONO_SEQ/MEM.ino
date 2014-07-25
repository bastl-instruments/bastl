

PROGMEM prog_uint16_t clearTo[]={
 0,0,0,0,0,0};
/*

 osc
 - pitch - freq - res 
 - rate - reso - amt - 
 - a -s- r  - když s==0 d=r
 - oscCharacter  / lfoCharacter / adsrCharacter 5
 
 -osc character
 ->wavetable 2 - filter/fm 1 - wavetable 2 bity =>5
 -lfo character
 ->shape 3 - destination 2 =>5
 
 -adsr character
 -> amt - 4 1 dest decay 3 
 
 */

// variable names
//page 0


//osc mod-freq amt
//atck decay mod type // 2+2+3

#define PAGE_G 0
#define PAGE_B 1


#define VARIABLES_PER_PAGE 3

#define OSC_TOP 0 //  1023 - 10
#define OSC_LEFT 1 // 255 - 8
#define OSC_RIGHT 2// 255 - 8
//26
#define OSC_CHAR 3 // 7 - 3
#define LFO_CHAR 4 // 255 - 8
#define ADSR_CHAR 5 // 255 -8


#define OSC_TYPE 0
#define OSC_PARAM_1 1
#define OSC_PARAM_2 2

#define LFO_AMT 3
#define LFO_RATE 4
#define LFO_DEST 5
#define LFO_SHAPE 6

#define ADSR_TYPE 7
#define ADSR_ATTACK 9
#define ADSR_RELEASE 8


#define NUMBER_OF_PARAMS 10

const unsigned char bits[NUMBER_OF_PARAMS]={3,3,2,2,2,2,2,2,3,3};
const unsigned char byteN[NUMBER_OF_PARAMS]={0,0,0,1,1,1,1,2,2,2};
const unsigned char bitOffset[NUMBER_OF_PARAMS]={5,2,0,6,4,2,0,6,3,0};

unsigned char var[3];

unsigned char getBits(unsigned char _subVar){
  unsigned char _val=0;
  for(int i=0;i<bits[_subVar];i++){
    bitWrite(_val,i,bitRead(var[byteN[_subVar]],bitOffset[_subVar]+i));
  }
  return _val;
}


//REC
// tempo
// lenght
// groove


// rate
// stretch
// cut

// mystery
// crush
// volume


#define NUMBER_OF_PATTERNS 6
#define NUMBER_OF_BITS_PER_STEP 3


//19

// 4*8= 32+13= 45 bitu => 6 bytů

//6 zvuku *6 bytů = 36 bytů / preset

#define KNOB_BITS 10


  PROGMEM prog_uint16_t maxValue[]={
  255,255,255, 255,255,255}; //const
  //10+10+10+8+8+8 = 30+24 = 54 / 8

//3bit per step - pattern = 32*3bit = 96bit = 12 bytu

#define NUMBER_OF_VARIABLES 6 
#define NUMBER_OF_BYTES 6
#define NUMBER_OF_PATTERN_BYTES 12
#define NUMBER_OF_PRESETS 6

//5patterns*12 bytes = 60 bytes

#define PRESET_SIZE 160 //7*7=49+60=109

#define NUMBER_OF_SOUNDS 7

#define CHANNEL_BYTE 1023

#define TEMPO_BYTE 156
#define GROOVE_BYTE 157


unsigned char variableDepth[NUMBER_OF_VARIABLES]={8,8,8,8,8,8};



int maxVal(int _maxVal){
  return  pgm_read_word_near(maxValue+_maxVal);
}

//const unsigned char byteCoordinate[NUMBER_OF_VARIABLES]={0, 1, 2, 3, 4, 5};
//const unsigned char bitCoordinate[NUMBER_OF_VARIABLES]={0, 2, 4, 6, 6, 6};
unsigned char currentPreset=0;
/*
int buffer[NUMBER_OF_SOUNDS][NUMBER_OF_VARIABLES];
 unsigned char bufferP[NUMBER_OF_STEPS];
 */

unsigned char variable[NUMBER_OF_SOUNDS][NUMBER_OF_BYTES];
unsigned char pattern[NUMBER_OF_PATTERNS][NUMBER_OF_PATTERN_BYTES];


void initMem(){

  calculateBitDepth();
  ShouldIClearMemory();
  if(hw.factoryClear()) clearMemmory(), hw.factoryCleared();
  hw.initialize(DEFAULT);
  loadPreset(0);

}

void calculateBitDepth(){
/*
  for(int i=0;i<NUMBER_OF_VARIABLES;i++){ // calculate bitDepth according to the maximum value
    int x=0;
    while(maxVal(i)-pow(2,x)>=0) x++;
    variableDepth[i]=x ;
  }

 Serial.print("start");
  for(int i=0;i<NUMBER_OF_VARIABLES;i++){
    int sum=0;
    for(int j=0;j<i;j++){
      sum+=variableDepth[j];
    }
    byteCoordinate[i]=sum/8;
    
    bitCoordinate[i]=sum%8;
    Serial.print(bitCoordinate[i]);
    Serial.print(", ");
  } 
  Serial.print("end");
*/
}

int getVar(unsigned char _SOUND, unsigned char _VARIABLE){
 return variable[_SOUND][_VARIABLE];
/*
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
*/
}

void setVar(unsigned char _SOUND, unsigned char _VARIABLE, int _value){
variable[_SOUND][_VARIABLE]=_value;
/*
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
*/
}

#define PATTERN_OFFSET 42
void storePreset(unsigned char index) {

  int offset = index * PRESET_SIZE;
  for (int j = 0; j < NUMBER_OF_SOUNDS; j++) {
    for (int k = 0; k < NUMBER_OF_BYTES; k++) {
      EEPROM.write(offset + ((NUMBER_OF_BYTES * j) + k), variable[j][k]);
    }
  }

  for (int j = 0; j < NUMBER_OF_PATTERNS-1; j++) {
    for (int k = 0; k < NUMBER_OF_PATTERN_BYTES; k++) {
      EEPROM.write(offset +PATTERN_OFFSET+ ((NUMBER_OF_PATTERN_BYTES * j) + k),pattern[j][k]);
    }
  }
  EEPROM.write(offset + TEMPO_BYTE,seq.getTempo());
  EEPROM.write(offset + GROOVE_BYTE,seq.getGrooveAmount());
}

void loadPreset(unsigned char index) {

  currentPreset=index;
  int offset = index * PRESET_SIZE;

  for (int j = 0; j < NUMBER_OF_SOUNDS; j++) {
    for (int k = 0; k < NUMBER_OF_BYTES; k++) {
      variable[j][k]=EEPROM.read(offset + ((NUMBER_OF_BYTES * j) + k));

    }
  }
  for (int j = 0; j < NUMBER_OF_PATTERNS-1; j++) {
    for (int k = 0; k < NUMBER_OF_PATTERN_BYTES; k++) {
      pattern[j][k]=EEPROM.read(offset +PATTERN_OFFSET+ ((NUMBER_OF_PATTERN_BYTES * j) + k));
    }
  }

  seq.setTempo(EEPROM.read(offset + TEMPO_BYTE));
  seq.setGrooveAmount(EEPROM.read(offset + GROOVE_BYTE));
}


//unsigned char initialSampe[6]={0,2,3,4,6,8};

void clearMemmory(){

  for(int x=0;x<NUMBER_OF_PRESETS;x++){
    loadPreset(x);
    for(int i=0;i<NUMBER_OF_SOUNDS;i++){
      for(int j=0;j<NUMBER_OF_VARIABLES;j++){
        setVar(i,j, pgm_read_word_near(clearTo+j));
      }
    }
    for (int j = 0; j < NUMBER_OF_PATTERNS; j++) {
      for (int k = 0; k < NUMBER_OF_PATTERN_BYTES; k++) {
        pattern[j][k]=0;
      }
    }
    //  tempo=120;
    seq.setTempo(120);
    seq.setGrooveAmount(0);
    // EEPROM.write(offset + TEMPO_BYTE,seq.getTempo());
    //EEPROM.write(offset + GROOVE_BYTE,seq.getGrooveAmount());
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
    setMidiChannel(1);
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

unsigned char getStep(unsigned char _PATTERN,unsigned char _STEP){

  unsigned char _byte=(_STEP*NUMBER_OF_BITS_PER_STEP)/8;
  unsigned char _bit=(_STEP*NUMBER_OF_BITS_PER_STEP)%8;
  unsigned char _OUT=0;
  for(int i=0;i<NUMBER_OF_BITS_PER_STEP;i++){
    bitWrite(_OUT,i, bitRead( pattern[_PATTERN][_byte],_bit));
    _bit++;
    if(_bit>=8) _bit=0,_byte++;
  }
  return _OUT;

}

void setStep( unsigned char _PATTERN,unsigned char _STEP,unsigned char _IN){

  unsigned char _byte=(_STEP*NUMBER_OF_BITS_PER_STEP)/8;
  unsigned char _bit=(_STEP*NUMBER_OF_BITS_PER_STEP)%8;

  for(int i=0;i<NUMBER_OF_BITS_PER_STEP;i++){
    bitWrite(pattern[_PATTERN][_byte],_bit,bitRead(_IN,i));
    _bit++;
    if(_bit>=8) _bit=0,_byte++;
  }


}


