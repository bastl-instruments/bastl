

#include <Arduino.h>
#include <portManipulations.h>
#include <SEKVOJ_HW.h>




int main(void) {

  init();
  setup();

  while(true) {
    loop();
  }
}



extern sekvojHW hardware;


void setup() {


	hardware.setup();
	Serial.begin(115200);

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



