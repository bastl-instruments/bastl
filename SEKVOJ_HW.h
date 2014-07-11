
#ifndef SEKVOJHW_H_
#define SEKVOJHW_H_


#define SHIFTREGISTER_SER C,5
#define SHIFTREGISTER_RCK B,1
#define SHIFTREGISTER_SRCK B,0
//#include <shiftRegisterFast.h>

#define BUTTONCOL_0 C,1
#define BUTTONCOL_1 C,0
#define BUTTONCOL_2 D,7
#define BUTTONCOL_3 D,6

static const uint8_t updateFreq = 50; // in Hertz



class sekvojHW {



public:

	enum buttonState{pressed,released};

	//sekvojHW(){};
	void setup();

	void leds_printStates();
	void leds_setStates(uint16_t ledStates[]);
	void leds_allOff();
	void leds_update();
	void leds_updateNextRow();
	void led_setOn(uint8_t number);
	void led_setOff(uint8_t number);

	void buttons_update();
	void buttons_printStates();
	buttonState button_getState(uint8_t number);

	void display_start();
	void display_clear();

private:

	void display_sendCommand(uint8_t command);
	void display_sendData(uint8_t data);
	void display_send(uint8_t byte);
	void display_enable();

	uint16_t ledStates[4];
	uint16_t buttonStates[4];



};




#endif
