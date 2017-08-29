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
								   playerModeChangedCallback_(0),
								   triggerLength_(0),
								   swingValue_(0)
{}

void PlayerSettings::resetManipulatedPatterns() {
	for (unsigned char pattern = 0; pattern < 4; pattern++) {
		manipulatedPatterns_[pattern] = 0;
	}
	SETBIT(manipulatedPatterns_[currentPattern_ / 16], currentPattern_ % 16, true);
}

void PlayerSettings::setCurrentPattern(unsigned char pattern) {
	unsigned char originalPattern = currentPattern_;
	if (currentPattern_ != pattern) {
		currentPattern_ = pattern;
		SETBIT(manipulatedPatterns_[pattern / 16], pattern % 16, true);
		patternChangedCallback_(originalPattern, currentPattern_);
		settingsChangedCallback_();
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
	data[8] = triggerLength_;
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
	triggerLength_ = 			data[8];
	resetManipulatedPatterns();
}

unsigned char PlayerSettings::getRecordQuantizationSize() {
	unsigned char quantizationRate = 1;
	for (unsigned char powNumber = 0; powNumber < (unsigned char)recordQunatizationType_; powNumber++) {
		quantizationRate *= 2;
	}
	return quantizationRate;

}
