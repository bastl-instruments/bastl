

PROGMEM prog_uint16_t clearTo[]={
  370,64,255, 0,0,255};
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

#define RATE 0 //  1023 - 10
#define STRETCH 1 // 255 - 8
#define CUT 2// 255 - 8
//26
#define SAMPLE 3 // 7 - 3
#define CRUSH 4 // 255 - 8
#define VOLUME 5 // 255 -8
//19

// 4*8= 32+13= 45 bitu => 6 bytů

//6 zvuku *6 bytů = 36 bytů / preset

#define KNOB_BITS 10


PROGMEM prog_uint16_t maxValue[]={
  1023,255,255, 7,255,255}; //const


#define NUMBER_OF_VARIABLES 6
#define NUMBER_OF_BYTES 6
#define NUMBER_OF_PRESETS 3

#define PRESET_SIZE 36

#define NUMBER_OF_SOUNDS 6

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

        if(j==SAMPLE)  setVar(i,j,i*3);
        else if (j==RATE && i>=3) setVar(i,j, pgm_read_word_near(clearTo+j)+350);
        else if (j==STRETCH && i>=3) setVar(i,j, pgm_read_word_near(clearTo+j)+64);
        else setVar(i,j, pgm_read_word_near(clearTo+j));


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

/*

 void debug(){
 randomSeed(analogread(0));
 
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



