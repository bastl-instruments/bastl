/*
  attiny midi clock to cv clock converter
 - works for gb clock - nanoloop
 - works for korg cv snyc - volca, monotribe
 TODO:
 - din cync 24 / 48
 - cv clock(din sync) to midi clock converter
 
 */
#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 6); // RX, TX

boolean cvState;

unsigned char cvPin=1; 
unsigned char speedPin=4;
unsigned char dinCVPin=3;
unsigned char startPin=3;
unsigned char divider;

boolean doubleClock;
unsigned char multiplier;
uint32_t clockTime, subTime, lastTime, quarterClockTime;
void setup()  
{
  // Open serial communications and wait for port to open:
  mySerial.begin(31250);
  pinMode(cvPin,OUTPUT);


  pinMode(speedPin,INPUT);
  digitalWrite(speedPin,HIGH);
  pinMode(dinCVPin,INPUT);
  digitalWrite(dinCVPin,HIGH);

  if(!digitalRead(speedPin)){
    if(digitalRead(dinCVPin)) divider=2; // gb
    else divider=12; // korg
    doubleClock=false;
  }
  else{
    if(digitalRead(dinCVPin))  multiplier=4; // 48
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
  pinMode(speedPin,INPUT_PULLUP);
}


int counter;
boolean render;
int subMult;
void loop() // run over and over
{
  if(doubleClock){
    uint32_t _time=micros();

    if((_time-subTime)>=quarterClockTime){
      subMult++;
      if(subMult<multiplier){
        subTime=_time; 
        if(render){
          if((subMult%2)==1) cvState=false;
          else cvState=true;
          digitalWrite(cvPin,cvState);
        } 
      }
    }
  }

  if (mySerial.available() > 0) {

    // read the incoming byte:
    byte incomingByte = mySerial.read();
    //  Serial.write(incomingByte);


    if(incomingByte==0xF8){ //clock
      if(doubleClock){
        /*
        if(subMult<(multiplier-1)){
          while((micros()-subTime)<1000);
          while(subMult<(multiplier-1)){
            subMult++;
            if((subMult%2)==1) cvState=false;
            else cvState=true;
            digitalWrite(cvPin,cvState);
            delayMicroseconds(1000);
          }
        }
        */
        uint32_t time=micros();
        clockTime=time-lastTime;
        quarterClockTime=clockTime/multiplier;
        lastTime=time;
        cvState=true;
        digitalWrite(cvPin,cvState);
        subTime=time;
        subMult=0;
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

}










