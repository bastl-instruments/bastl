

#include <Arduino.h>
#include <portManipulations.h>
#include <sekvojHW.h>
#include <SdFat.h>



SdFat sd;
SdFile file;

int main(void) {

  init();
  setup();

  while(true) {
    loop();
  }
}



extern sekvojHW hardware;


void setup() {



	Serial.begin(115200);


	hardware.init();

	hardware.setLED(16,sekvojHW::ON);
	hardware.setLED(17,sekvojHW::BLINK);
	hardware.setLED(18,sekvojHW::BLINK_INVERT);


	delayMicroseconds(10000);

	hardware.printLEDStates();



}


void loop() {

	if (hardware.getButtonState(16) == sekvojHW::DOWN ) {
		hardware.setLED(16,sekvojHW::ON);
	} else {
		hardware.setLED(16,sekvojHW::OFF);
	}




}



