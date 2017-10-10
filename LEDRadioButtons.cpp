#include "LEDRadioButtons.h"

void LEDRadioButtons::init( unsigned char * buttonIndexes, unsigned char count) {
	RadioButtons::init(buttonIndexes, count);
}

void LEDRadioButtons::setSelectedButton(unsigned char selectedButton) {
	RadioButtons::setSelectedButton(selectedButton);
	LEDsAndButtonsHWWrapper::setLED(buttonIndexes_[selectedButton_], ILEDHW::ON);
}

void LEDRadioButtons::resetSelection() {
	if (buttonSelected_) {
		LEDsAndButtonsHWWrapper::setLED(buttonIndexes_[selectedButton_], ILEDHW::OFF);
	}
	RadioButtons::resetSelection();
}

