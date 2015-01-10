#include "PlayerSettings.h"

PlayerSettings::PlayerSettings() : drumInstrumentEventTypes_(0),
								   instrumentStatuses_(255),
								   currentPattern_(0),
								   patternChangedCallback_(0),
								   recordQunatizationType_(_1_64),
								   multiplication_(_8),
								   multiplicationChangedCallback_(0),
								   bpm_(120),
								   playerMode_ (MASTER),
								   playerModeChangedCallback_(0)
{}


void PlayerSettings::setCurrentPattern(unsigned char pattern) {
	unsigned char originalPattern = currentPattern_;
	if (currentPattern_ != pattern) {
		currentPattern_ = pattern;
		if (patternChangedCallback_) {
			patternChangedCallback_(originalPattern, currentPattern_);
		}
		if (settingsChangedCallback_) {
			settingsChangedCallback_();
		}
	}
}

void PlayerSettings::getInByteArray(unsigned char * data) {
	data[0] = drumInstrumentEventTypes_;
	data[1] = instrumentStatuses_;
	data[2] = currentPattern_;
	data[3] = (unsigned char)recordQunatizationType_;
	data[4] = (unsigned char)multiplication_;
	data[5] = playerMode_;
	data[6] = (unsigned char)bpm_;
	data[7] = (unsigned char)(bpm_ >> 8);
}

void PlayerSettings::loadFromByteArray(unsigned char * data) {
	drumInstrumentEventTypes_ = data[0];
	instrumentStatuses_ = 		data[1];
	currentPattern_ = 			data[2];
	recordQunatizationType_ = 	(QuantizationType)(data[3]);
	multiplication_ = 			(MultiplicationType)(data[4]);
	playerMode_ = 				(PlayerMode)(data[5]);
	bpm_ = 						data[6];
	bpm_ +=						(((unsigned int)data[7]) << 8);
}
