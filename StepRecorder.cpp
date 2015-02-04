/*
 * StepRecorder.cpp
 *
 *  Created on: Aug 28, 2014
 *      Author: bastl
 */

#include "StepRecorder.h"

StepRecorder::StepRecorder() : player_(0), memory_(0), playerSettings_(0), stepper_(0), record_(true) {
	for (unsigned char instrument = 0; instrument < 6; instrument++) {
		recordInstrumentStatuses_[instrument] = false;
	}
}

void StepRecorder::init(Player* player, IStepMemory * memory, PlayerSettings* playerSettings, BastlStepper * stepper) {
	player_ = player;
	memory_ = memory;
	playerSettings_ = playerSettings;
	stepper_ = stepper;
}

unsigned char StepRecorder::getCurrentQuantizedSubstep(unsigned char instrumentIndex) {
	unsigned char currentStep = player_->getCurrentInstrumentStep(instrumentIndex);
	unsigned char currentSubStep = player_->getCurrentInstrumentSubStep(instrumentIndex);

	//Start of quantization evaluation
	unsigned char quntizationSetting = (unsigned char) (playerSettings_->getRecordQuantizationType());
	unsigned char quantizationRate = 1;
	for (unsigned char powNumber = 0; powNumber < quntizationSetting; powNumber++) {
		quantizationRate *= 2;
	}

	int doubleSubstepValue = (currentStep * 4 + currentSubStep) * 2;
	doubleSubstepValue += stepper_->isCloserToNextStep() ? quantizationRate : 0;
	doubleSubstepValue = (doubleSubstepValue / (quantizationRate * 2)) * quantizationRate;
	return (unsigned char) doubleSubstepValue;
}

unsigned int StepRecorder::startRecordNote(unsigned char instrumentIndex) {

	recordInstrumentStatuses_[instrumentIndex] = true;
	if (playerSettings_->getDrumInstrumentEventType(instrumentIndex) == PlayerSettings::TRIGGER) {
		stopRecordNote(instrumentIndex);
	}
	player_->playNote(instrumentIndex, DrumStep::NORMAL);
	return getCurrentQuantizedSubstep(instrumentIndex);
}

void StepRecorder::update() {

	for (unsigned char instrument = 0; instrument < 6; instrument++) {
		if (playerSettings_->getDrumInstrumentEventType(instrument) == PlayerSettings::GATE) {
			if (recordInstrumentStatuses_[instrument]) {
				recordSubstep(instrument, getCurrentQuantizedSubstep(instrument));
				player_->playNote(instrument, DrumStep::NORMAL);
			}
		}
	}
}

void StepRecorder::recordSubstep(unsigned char instrumentIndex, unsigned char subStepIndex) {

	unsigned char currentStep = (unsigned char)(subStepIndex / 4);
	unsigned char currentSubStep = (unsigned char)(subStepIndex % 4);
	//End of quantization evaluation
	DrumStep manipulatedDrumStep = memory_->getDrumStep(instrumentIndex, currentStep);
	if (manipulatedDrumStep.isMuted()) {
		manipulatedDrumStep.setMuted(false);
		for (unsigned char subStep = 0; subStep < 4; subStep++) {
			manipulatedDrumStep.setSubStep(subStep, DrumStep::OFF);
		}
	}
	manipulatedDrumStep.setSubStep(currentSubStep, DrumStep::NORMAL);
	memory_->setDrumStep(instrumentIndex, currentStep, manipulatedDrumStep);

}

void StepRecorder::stopRecordNote(unsigned char instrumentIndex) {

	if (recordInstrumentStatuses_[instrumentIndex]) {
		recordInstrumentStatuses_[instrumentIndex] = false;
		recordSubstep(instrumentIndex, getCurrentQuantizedSubstep(instrumentIndex));
		player_->playNote(instrumentIndex, DrumStep::NORMAL);
	}
}

void StepRecorder::recordMIDINote(unsigned char channel, unsigned char note) {
	unsigned char instrumentID = 0;
	bool instrumentMapped = false;//playerSettings_->getDrumInstrumentIndexFromMIDIMessage(channel, note, instrumentID);
	if (instrumentMapped) {
		startRecordNote(instrumentID);
		stopRecordNote(instrumentID);
	}
}

#include <StepRecorder.h>

