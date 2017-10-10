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
								   swingValue_(0),
								   patternMomentary_(false)
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
	data[9] =  patternMomentary_ ? 1 : 0;
	data[10] = swingValue_;
}

void PlayerSettings::loadFromByteArray(unsigned char * data) {
	drumInstrumentEventTypes_ = data[0];
	instrumentStatuses_ = 		data[1];
	currentPattern_ = 			data[2];
	recordQunatizationType_ = 	(data[3] > 4) ? _1_64 :(QuantizationType)(data[3]);
	multiplication_ = 			(data[4] > 4) ? _8 :(MultiplicationType)(data[4]);
	playerMode_ = 				(data[5] > 1) ? MASTER : (PlayerMode)(data[5]);
	bpm_ = 						data[6];
	bpm_ +=						(((unsigned int)data[7]) << 8);
	triggerLength_ = 			data[8];
	patternMomentary_ = 		data[9]& 1 != 0;
	swingValue_ = 				data[10];

	// Here comes the value validation:
	swingValue_ = (swingValue_ > 55) ? 0 : swingValue_;
	triggerLength_ = (triggerLength_ > 7) ? 0 : triggerLength_;
	bpm_ = (bpm_ > 500) ? 120 : bpm_;
	currentPattern_ = (currentPattern_ > 64) ? 0 : currentPattern_;
	instrumentStatuses_ = (instrumentStatuses_ > 64) ? 0 : instrumentStatuses_;
	drumInstrumentEventTypes_ = (drumInstrumentEventTypes_ > 64) ? 0 : drumInstrumentEventTypes_;


	resetManipulatedPatterns();
}

unsigned char PlayerSettings::getRecordQuantizationSize() {
	unsigned char quantizationRate = 1;
	for (unsigned char powNumber = 0; powNumber < (unsigned char)recordQunatizationType_; powNumber++) {
		quantizationRate *= 2;
	}
	return quantizationRate;

}
