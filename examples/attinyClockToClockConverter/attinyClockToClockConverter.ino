/*
  attiny midi clock to cv clock converter
 - works for gb clock - nanoloop
 - works for korg cv snyc - volca, monotribe
 TODO:
 - din cync 24 / 48
 - cv clock(din sync) to midi clock converter
 volca clock je 2 ppqn
 midi clock je 24 ppqn
 gameboy je 12 ppqn
 din sync je 24 nebo 48 ppqn
 
 
 */
#include <SoftwareSerial.h>

//SoftwareSerial mySerial(0, 6); // RX, TX

SoftwareSerial mySerial(6, 0);

unsigned char divider=6;

unsigned char multiplier=2; // use 12 for converting from volca to midi

boolean cvState;

unsigned char cvPin=1; 
unsigned char speedPin=4;
unsigned char dinCVPin=3;
unsigned char startPin=3;

boolean doubleClock;

long clockTime, subTime, lastTime, quarterClockTime;
void setup()  
{
  // Open serial communications and wait for port to open:
  mySerial.begin(31250);
  pinMode(cvPin,OUTPUT);

  /*
  pinMode(speedPin,INPUT);
   digitalWrite(speedPin,HIGH);
   pinMode(dinCVPin,INPUT);
   digitalWrite(dinCVPin,HIGH);
   
   if(!digitalRead(dinCVPin)){
   if(digitalRead(speedPin)) divider=2; // gb
   else divider=12; // korg
   doubleClock=false;
   }
   else{
   if(digitalRead(speedPin))  multiplier=4; // 48
   else multiplier=2; // 24
   doubleClock=true;
   //2
   }
   pinMode(speedPin,OUTPUT);
   for(int i=0;i<4;i++){
   digitalWrite(speedPin,cvState);
   cvState=!cvState;
   delay(100);
   }
   */
  pinMode(speedPin,INPUT_PULLUP);

  //  pinMode(startPin,OUTPUT);
}


int counter;
boolean render=false;

boolean lastInClock,inClock;
boolean firstClock;
void loop(){
 // divider=6; // for volca
//  multiplier=2; // for midi // for converting from volca to midi use 12


  lastInClock=inClock;
  inClock=digitalRead(speedPin);
  if(inClock && !lastInClock){

    if(!render) mySerial.write(0xFA); //midi start
    render=true;

    counter++;
    if(counter>=divider) cvState=true, counter=0;
    else cvState=false;
    // cvState=!cvState;
    if(render) digitalWrite(cvPin,cvState);




    long time=micros();
    clockTime=time-lastTime;
    quarterClockTime=clockTime/multiplier;
    subTime=time;
    lastTime=time;
    // cvState=true;
    // digitalWrite(cvPin,HIGH);
    if(firstClock) mySerial.write(0xF8);
    mySerial.write(0xF8); //clock
    firstClock=true;
    subTime=time;

  }

  long _time=micros();

  if((_time-subTime)>=1000000) { // timeout time before sends the stop
    if(render) mySerial.write(0xFC); // midi stop
    render=false;
  }
 if(firstClock){
  if((_time-subTime)>=quarterClockTime){ //100 slows the in between pulse a bit (by 1ms) to make sure it wont send more midi clock
    subTime=_time; 
    
    if(render){
      //  cvState=!cvState;
      if(firstClock) mySerial.write(0xF8), firstClock=false; //clock
      


      //if(incomingByte==){
      // digitalWrite(cvPin,cvState);
      // } 
    }

  }
 }


  /*

   if(doubleClock){
   long _time=micros();
   
   if((_time-subTime)>=quarterClockTime){
   subTime=_time; 
   if(render){
   cvState=!cvState;
   digitalWrite(cvPin,cvState);
   } 
   }
   }
   
   if (mySerial.available() > 0) {
   
   // read the incoming byte:
   byte incomingByte = mySerial.read();
   //  Serial.write(incomingByte);
   
   
   if(incomingByte==0xF8){ //clock
   if(doubleClock){
   long time=micros();
   clockTime=time-lastTime;
   quarterClockTime=clockTime/multiplier;
   lastTime=time;
   cvState=true;
   digitalWrite(cvPin,HIGH);
   subTime=time;
   //event=0;
   }
   
   else{
   counter++;
   if(counter>=divider) cvState=true, counter=0;
   else cvState=false;
   // cvState=!cvState;
   if(render) digitalWrite(cvPin,cvState);
   }
   }
   else if(incomingByte==0xFA){ //start
   if(doubleClock){
   pinMode(startPin,OUTPUT);
   digitalWrite(startPin,HIGH); 
   }
   render=true;
   counter=divider-1;
   // startPressed=true;
   }
   else if(incomingByte==0xFC){ //stop
   if(doubleClock){
   pinMode(startPin,OUTPUT);
   digitalWrite(startPin,LOW); 
   }
   render=false;
   }
   }
   */
}














