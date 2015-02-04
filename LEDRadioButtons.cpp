#include "LEDRadioButtons.h"

LEDRadioButtons::LEDRadioButtons(ILEDsAndButtonsHW *hwLayer, unsigned char * buttonIndexes, unsigned char count) :
	RadioButtons::RadioButtons(hwLayer, buttonIndexes, count), ledHWLayer_(hwLayer)
{}

void LEDRadioButtons::setSelectedButton(unsigned char selectedButton) {
	RadioButtons::setSelectedButton(selectedButton);
	ledHWLayer_->setLED(buttonIndexes_[selectedButton_], ILEDHW::ON);
}

void LEDRadioButtons::resetSelection() {
	if (buttonSelected_) {
		ledHWLayer_->setLED(buttonIndexes_[selectedButton_], ILEDHW::OFF);
	}
	RadioButtons::resetSelection();
}

