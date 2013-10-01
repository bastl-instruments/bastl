

PROGMEM prog_uchar clearTo[]={
  0,255,0,0, 0,0,1,0, 0,0,1,0, 0,0,1,0, 1,0,0,0, };

// variable names
//page 0

#define CARRIER_PAGE 0
#define MOD_1_PAGE 1
#define MOD_2_PAGE 2
#define MOD_3_PAGE 3
#define LFO_PAGE 4

#define CARRIER_FREQ 0 // 1023
#define MOD_AMT 1 // 255
#define ALGORITHM 2 //7
#define CARRIER_WAVETABLE 3 // 3
//page R

#define MOD_1_DETUNE 4 // 127
#define MOD_1_AMT 5 // 255
#define MOD_1_HARMONIC 6 // 15
#define MOD_1_WAVETABLE 7 // 3
// page G

#define MOD_2_DETUNE 8 //127
#define MOD_2_AMT 9 //255
#define MOD_2_HARMONIC 10 //15
#define MOD_2_WAVETABLE 11 //3
// page B

#define MOD_3_DETUNE 12 //127
#define MOD_3_AMT 13 //255
#define MOD_3_HARMONIC 14 //15
#define MOD_3_WAVETABLE 15 //3
//page RG
#define LFO_RATE 16 // 255
#define LFO_AMT 17 // 255
#define LFO_DEST 18 // 255
#define LFO_WAVETABLE 19 // 7

#define KNOB_BITS 10
// page RB

// page GB

// page RGB

PROGMEM prog_uint16_t maxValue[]={
  127,255,7,3,255 ,255,15,3,255,255,15,3,255,255,15,3,255,255,255,7}; //const

//  10+8+3+2+7+8+4+2+7+8+4+2+7+8+4+2+8+8+8+2 = 115
//107 /8 = 14.34 => 15 bytes


//7*14=98
//

#define NUMBER_OF_VARIABLES 20
#define NUMBER_OF_BYTES 16
#define NUMBER_OF_PRESETS 3
#define PRESET_SIZE 120

#define NUMBER_OF_PATTERNS 4
#define NUMBER_OF_STEPS 4

#define NUMBER_OF_SOUNDS 8

#define CHANNEL_BYTE 1023

//unsigned char pattern[NUMBER_OF_PATTERNS][NUMBER_OF_STEPS];


unsigned char variableDepth[NUMBER_OF_VARIABLES];//={1,2,3,6,4,8,2,3,1,5,3,7,8,6,7};
unsigned int var[NUMBER_OF_VARIABLES];


unsigned int maxVal(unsigned int _maxVal){
  return  pgm_read_word_near(maxValue+_maxVal);
}

unsigned char byteCoordinate[NUMBER_OF_VARIABLES];
unsigned char bitCoordinate[NUMBER_OF_VARIABLES];
unsigned char currentPreset;
/*
unsigned int buffer[NUMBER_OF_SOUNDS][NUMBER_OF_VARIABLES];
 unsigned char bufferP[NUMBER_OF_STEPS];
 */

unsigned char variable[NUMBER_OF_SOUNDS][NUMBER_OF_BYTES];

void initMem(){

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

  ShouldIClearMemory();
  loadPreset(0);

}

unsigned int getVar(unsigned char _SOUND, unsigned char _VARIABLE){

  unsigned int _value=0;
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

void setVar(unsigned char _SOUND, unsigned char _VARIABLE, unsigned int _value){

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

/*
boolean GetStep(unsigned char _PATTERN,unsigned char _STEP,unsigned char _SOUND){
 boolean isStep;
 
 isStep=bitRead(pattern[_PATTERN][_STEP],_SOUND);
 
 return isStep;
 }
 
 void SetStep(unsigned char _PATTERN,unsigned char _STEP,unsigned char _SOUND, boolean _ISSTEP){
 
 bitWrite(pattern[_PATTERN][_STEP],_SOUND,_ISSTEP);
 
 }
 
 */


void storePreset(unsigned char index) {

  int offset = index * PRESET_SIZE;
  for (int j = 0; j < NUMBER_OF_SOUNDS; j++) {
    for (int k = 0; k < NUMBER_OF_BYTES; k++) {
      EEPROM.write(offset + ((NUMBER_OF_BYTES * j) + k), variable[j][k]);
    }
  }
  // Serial.println("store");
  /*
  int offset2= NUMBER_OF_SOUNDS*NUMBER_OF_BYTES;
   
   for (int j = 0; j < NUMBER_OF_PATTERNS; j++) {
   for (int k = 0; k < NUMBER_OF_STEPS; k++) {
   
   EEPROM.write(offset + offset2 + ((NUMBER_OF_STEPS * j) + k), pattern[j][k]);
   }
   }
   */
  // EEPROM.write(offset + 62,tempo);

}

void loadPreset(unsigned char index) {

  currentPreset=index;
  int offset = index * PRESET_SIZE;

  for (int j = 0; j < NUMBER_OF_SOUNDS; j++) {
    for (int k = 0; k < NUMBER_OF_BYTES; k++) {
      variable[j][k]=EEPROM.read(offset + ((NUMBER_OF_BYTES * j) + k));
    }
  }
  // Serial.println("LOAD");
  /*
  int offset2= NUMBER_OF_SOUNDS*NUMBER_OF_BYTES;
   
   for (int j = 0; j < NUMBER_OF_PATTERNS; j++) {
   for (int k = 0; k < NUMBER_OF_STEPS; k++) {
   pattern[j][k]=EEPROM.read(offset + offset2 + ((NUMBER_OF_STEPS * j) + k));
   }
   }
   */
  // tempo=EEPROM.read(offset + 62);
}



void clearMemmory(){
  for(int x=0;x<NUMBER_OF_PRESETS;x++){
    loadPreset(x);
    for(int i=0;i<NUMBER_OF_SOUNDS;i++){
      for(int j=0;j<NUMBER_OF_VARIABLES;j++){
        setVar(i,j, pgm_read_byte_near(clearTo+j));
      }
    }
    /*
    for(int i=0;i<NUMBER_OF_PATTERNS;i++){
     for(int k=0;k<NUMBER_OF_STEPS;k++){
     for(int l=0;l<NUMBER_OF_SOUNDS;l++){
     SetStep(i,k,l,false);
     }
     }
     }
     //  tempo=120;
     */
    storePreset(x);

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
  /*
  else {
   if(val1==inBet || val2==inBet) retVal=true;
   else retVal=false;
   }
   */
  return retVal;

}

unsigned int scale(unsigned int _value, unsigned char _originalBits, unsigned char _endBits){
  if(_originalBits >= _endBits) return _value >> (_originalBits - _endBits);
  else return _value << (_endBits - _originalBits);
}

/*

 void debug(){
 randomSeed(analogRead(0));
 
 Serial.begin(9600);
 Serial.println();
 
 for(int j=0;j<NUMBER_OF_PATTERNS;j++){
 for(int i=0;i<NUMBER_OF_STEPS;i++){
 bufferP[i]=random(255);
 for(int k=0;k<NUMBER_OF_SOUNDS;k++){
 Serial.print(bitRead(bufferP[i],k),DEC);
 SetStep(j,i,k,bitRead(bufferP[i],k));
 }
 }
 Serial.println();
 }
 
 Serial.println();
 Serial.println();
 for(int j=0;j<NUMBER_OF_PATTERNS;j++){
 for(int i=0;i<NUMBER_OF_STEPS;i++){
 for(int k=0;k<NUMBER_OF_SOUNDS;k++){
 Serial.print( GetStep(j,i,k),DEC);
 }
 }
 Serial.println();
 }
 
 StorePreset(0);
 // StorePreset(1);
 // ClearMemmory();
 // LoadPreset(1);
 
 Serial.println();
 for(int j=0;j<NUMBER_OF_PATTERNS;j++){
 for(int i=0;i<NUMBER_OF_STEPS;i++){
 for(int k=0;k<NUMBER_OF_SOUNDS;k++){
 Serial.print( GetStep(j,i,k),DEC);
 }
 }
 Serial.println();
 }
 
 Serial.println("original:");
 for(int j=0;j<NUMBER_OF_SOUNDS;j++){
 for(int i=0;i<NUMBER_OF_VARIABLES;i++){
 buffer[j][i]=random(0,maxVal(i));
 Serial.print(buffer[j][i],DEC);
 Serial.print(" , ");
 }
 Serial.println();
 }
 Serial.println();
 Serial.println("comprimed & decomprimed:");
 
 for(int j=0;j<NUMBER_OF_SOUNDS;j++){
 for(int i=0;i<NUMBER_OF_VARIABLES;i++){
 setVar(j,i,buffer[j][i]);
 }
 }
 for(int j=0;j<NUMBER_OF_SOUNDS;j++){
 for(int i=0;i<NUMBER_OF_VARIABLES;i++){
 Serial.print(getVar(j,i));
 Serial.print(" , ");
 }
 Serial.println();
 }
 
 
 Serial.println();
 Serial.println();
 Serial.println();
 StorePreset(0);
 // ClearMemmory();
 LoadPreset(0);
 Serial.println("loaded from eeprom:");
 for(int j=0;j<NUMBER_OF_SOUNDS;j++){
 for(int i=0;i<NUMBER_OF_VARIABLES;i++){
 Serial.print(getVar(j,i));
 Serial.print(" , ");
 }
 Serial.println();
 }
 
 
 
 
 Serial.println();
 for(int j=0;j<NUMBER_OF_PATTERNS;j++){
 for(int i=0;i<NUMBER_OF_STEPS;i++){
 for(int k=0;k<NUMBER_OF_SOUNDS;k++){
 Serial.print( GetStep(j,i,k),DEC);
 }
 }
 Serial.println();
 }
 
 }
 
 */









