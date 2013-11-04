#include <trinityHW.h>
trinityHW hw;
#include <MozziGuts.h>
#include <mozzi_analog.h>

void setup() {
  
  hw.setFreezeType(UNFREEZE_EXTERNALY);
  hw.initialize(DEFAULT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly: 
  hw.update();
  hw.update();
  for(int i=0;i<3;i++) { //print out big buttons
    Serial.print(" bB ");
    Serial.print("i");
    Serial.print(": ");
    Serial.print(hw.buttonState(i),DEC);
  }
  Serial.print(" sB 1:");
  Serial.print(hw.buttonState(SMALL_BUTTON_1),DEC);
  Serial.print(" sB 2:");
  Serial.print(hw.buttonState(SMALL_BUTTON_2),DEC);
  Serial.print(" eB 1:");
  Serial.print(hw.buttonState(EXTRA_BUTTON_1),DEC);
  Serial.print(" eB 2:");
  Serial.print(hw.buttonState(EXTRA_BUTTON_2),DEC);

  for(int i=0;i<3;i++) { //print out big buttons
    Serial.print("pot");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(hw.knobValue(i),DEC);
    delay(10);
  }
}

