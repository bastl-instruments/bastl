

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

	hardware.led_setState(16,sekvojHW::on);
	hardware.led_setState(17,sekvojHW::blinkStart);
	hardware.led_setState(18,sekvojHW::blinkEnd);


	delayMicroseconds(10000);

	hardware.leds_printStates();



}


void loop() {

	if (hardware.button_getState(16) == sekvojHW::pressed ) {
		hardware.led_setState(15,sekvojHW::on);
	} else {
		hardware.led_setState(15,sekvojHW::off);
	}




}



