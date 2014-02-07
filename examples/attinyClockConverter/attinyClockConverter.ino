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

unsigned char divider;

boolean doubleClock;
unsigned char multiplier;
long clockTime, subTime, lastTime, quarterClockTime;
void setup()  
{
  // Open serial communications and wait for port to open:
  mySerial.begin(31250);
  pinMode(cvPin,OUTPUT);


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
}


int counter;
boolean render;

void loop() // run over and over
{
  if(doubleClock){
    long time=micros();

    if(time-subTime>=quarterClockTime){
      subTime=time; 
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
        pinMode(cvPin,OUTPUT);
        digitalWrite(cvPin,HIGH); 
      }
      render=true;
      counter=divider-1;
      // startPressed=true;
    }
    else if(incomingByte==0xFC){ //stop
      if(doubleClock){
        pinMode(cvPin,OUTPUT);
        digitalWrite(cvPin,LOW); 
      }
      render=false;
    }
  }

}









