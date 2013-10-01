

PROGMEM prog_uchar clearTo[]={
  0,255,0,0, 0,0,1,0, 0,0,1,0, 0,0,1,0, 1,0,0,0, };

// variable names
//page 0

#define NUMBER_OF_INSTRUMENTS 3
#define NUMBER_OF_PATTERNS 4
#define NUMBER_OF_BITS_PER_STEP 6

#define NUMBER_OF_BYTES 24
#define NUMBER_OF_PRESETS 3
#define PRESET_SIZE 300

#define KNOB_BITS 10

#define TEMPO_BYTE 288
#define GROOVE_BYTE 289
#define INSTRUMENT_TYPE_BYTE 1022



unsigned char pattern[NUMBER_OF_INSTRUMENTS][NUMBER_OF_PATTERNS][NUMBER_OF_BYTES];
unsigned char instrumentType[NUMBER_OF_INSTRUMENTS];


unsigned char currentPreset;



void initMem(){

  ShouldIClearMemory();
  loadPreset(0);

}

unsigned char getStep(unsigned char _INSTRUMENT, unsigned char _PATTERN,unsigned char _STEP){

  unsigned char _byte=(_STEP*NUMBER_OF_BITS_PER_STEP)/8;
  unsigned char _bit=(_STEP*NUMBER_OF_BITS_PER_STEP)%8;
  unsigned char _OUT=0;
  for(int i=0;i<NUMBER_OF_BITS_PER_STEP;i++){
    bitWrite(_OUT,i, bitRead( pattern[_INSTRUMENT][_PATTERN][_byte],_bit));
    _bit++;
    if(_bit>=8) _bit=0,_byte++;
  }
  return _OUT;

}

void setStep(unsigned char _INSTRUMENT, unsigned char _PATTERN,unsigned char _STEP,unsigned char _IN){

  unsigned char _byte=(_STEP*NUMBER_OF_BITS_PER_STEP)/8;
  unsigned char _bit=(_STEP*NUMBER_OF_BITS_PER_STEP)%8;

  for(int i=0;i<NUMBER_OF_BITS_PER_STEP;i++){
    bitWrite(pattern[_INSTRUMENT][_PATTERN][_byte],_bit,bitRead(_IN,i));
    _bit++;
    if(_bit>=8) _bit=0,_byte++;
  }


}



void storePreset(unsigned char index) {

  int offset = index * PRESET_SIZE;

  for (int k = 0; k < NUMBER_OF_INSTRUMENTS; k++) {
    for (int j = 0; j < NUMBER_OF_PATTERNS; j++) {
      for (int i = 0; i < NUMBER_OF_BYTES; i++) {

        EEPROM.write(offset + ((NUMBER_OF_BYTES*NUMBER_OF_PATTERNS * k) + (NUMBER_OF_BYTES * j) + i), pattern[k][j][i]);
      }
    }
  }

  EEPROM.write(offset + TEMPO_BYTE,seq.getTempo());
   EEPROM.write(offset + GROOVE_BYTE,seq.getGrooveAmount());
  unsigned char _typeByte;
  for(int i=0;i<NUMBER_OF_INSTRUMENTS;i++) _typeByte=writeBits(_typeByte,2*i,2,instrumentType[i]);
  EEPROM.write(INSTRUMENT_TYPE_BYTE,_typeByte);

}

void loadPreset(unsigned char index) {

  currentPreset=index;
  int offset = index * PRESET_SIZE;

  for (int k = 0; k < NUMBER_OF_INSTRUMENTS; k++) {
    for (int j = 0; j < NUMBER_OF_PATTERNS; j++) {
      for (int i = 0; i < NUMBER_OF_BYTES; i++) {

        pattern[k][j][i]=EEPROM.read(offset + ((NUMBER_OF_BYTES*NUMBER_OF_PATTERNS * k) + (NUMBER_OF_BYTES * j) + i));
      }
    }
  }

  seq.setTempo(EEPROM.read(offset + TEMPO_BYTE));
  seq.setGrooveAmount(EEPROM.read(offset + GROOVE_BYTE));
  unsigned char _typeByte=EEPROM.read(INSTRUMENT_TYPE_BYTE);
  for(int i=0;i<NUMBER_OF_INSTRUMENTS;i++) instrumentType[i]=0,instrumentType[i]=getBits(_typeByte,2*i,2);
  // tempo=EEPROM.read(offset + 62);
}



void clearMemmory(){
  for(int x=0;x<NUMBER_OF_PRESETS;x++){
    loadPreset(x);
    for (int k = 0; k < NUMBER_OF_INSTRUMENTS; k++) {
      for (int j = 0; j < NUMBER_OF_PATTERNS; j++) {
        for (int i = 0; i < NUMBER_OF_BYTES; i++) {
          pattern[k][j][i]=0;
        }
      }
    }
    
    seq.setTempo(120);
    seq.setGrooveAmount(0);
    for(int p=0;p<3;p++) instrumentType[p]=p;
    storePreset(x);
   
  }
  
}


void IsThereChangeInMidiChannel(){
  // channel=i;
}


boolean ConditionToClear(){
  boolean _shouldClear=false;
  //if something or something else _shouldClear=true;
  if(hw.buttonState(BIG_BUTTON_1) && hw.buttonState(BIG_BUTTON_2) && hw.buttonState(BIG_BUTTON_3)) _shouldClear=true;
  return _shouldClear;
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
unsigned char bufferP[NUMBER_OF_BYTES];
 void debug(){
 randomSeed(analogRead(0));
 
 Serial.begin(9600);
 Serial.println();
 for(int k=0;k<NUMBER_OF_INSTRUMENTS;k++){
 for(int j=0;j<NUMBER_OF_PATTERNS;j++){
 for(int i=0;i<32;i++){
 bufferP[i]=random(63);
 Serial.print(bufferP[i],DEC);
 Serial.print(", ");
 setStep(k,j,i,bufferP[i]);
 
 }
 Serial.println();
 }
 Serial.println();
 }
 
 
 Serial.println();
 for(int k=0;k<NUMBER_OF_INSTRUMENTS;k++){
 for(int j=0;j<NUMBER_OF_PATTERNS;j++){
 for(int i=0;i<32;i++){
 
 Serial.print(getStep(k,j,i),DEC);
 Serial.print(", ");
 
 }
 Serial.println();
 }
 Serial.println();
 }
 
 
 Serial.println();
 Serial.println();
 
 
 
 storePreset(0);
 // StorePreset(1);
 // ClearMemmory();
 loadPreset(0);
 for(int k=0;k<NUMBER_OF_INSTRUMENTS;k++){
 for(int j=0;j<NUMBER_OF_PATTERNS;j++){
 for(int i=0;i<32;i++){
 
 Serial.print(getStep(k,j,i),DEC);
 Serial.print(", ");
 
 }
 Serial.println();
 }
 Serial.println();
 }
 
 
 Serial.println();
 Serial.println();
/*
 
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












