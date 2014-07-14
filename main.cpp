

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


	hardware.setup();

	hardware.led_setOn(17);
	hardware.led_setOn(18);
	hardware.led_setOn(19);
	hardware.led_setOn(21);




}


void loop() {

	if (hardware.button_getState(16) == sekvojHW::pressed ) {
		hardware.led_setOn(16);
	} else {
		hardware.led_setOff(16);
	}




}



