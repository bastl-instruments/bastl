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
		lastQuantizedSteps_[instrument] = 0;
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
	unsigned char quantizationRate = playerSettings_->getRecordQuantizationSize();

	int doubleSubstepValue = (currentStep * 4 + currentSubStep) * 2;
	doubleSubstepValue += stepper_->isCloserToNextStep() ? quantizationRate : 0;
	doubleSubstepValue = (doubleSubstepValue / (quantizationRate * 2)) * quantizationRate;
	return (unsigned char) doubleSubstepValue;
}

unsigned int StepRecorder::startRecordNote(unsigned char instrumentIndex) {

	recordInstrumentStatuses_[instrumentIndex] = true;
	lastQuantizedSteps_[instrumentIndex] = getCurrentQuantizedSubstep(instrumentIndex);
	if (playerSettings_->getDrumInstrumentEventType(instrumentIndex) == PlayerSettings::TRIGGER) {
		stopRecordNote(instrumentIndex);
	} else {
		recordSubsteps(instrumentIndex, lastQuantizedSteps_[instrumentIndex]);
	}
	player_->playNote(instrumentIndex, DrumStep::NORMAL);
	return lastQuantizedSteps_[instrumentIndex];
}

void StepRecorder::update() {

	for (unsigned char instrument = 0; instrument < 6; instrument++) {
		if (playerSettings_->getDrumInstrumentEventType(instrument) == PlayerSettings::GATE) {
			if (recordInstrumentStatuses_[instrument]) {
				unsigned char currentQuantizedStep = getCurrentQuantizedSubstep(instrument);
				if (currentQuantizedStep != lastQuantizedSteps_[instrument]) {
					recordSubsteps(instrument, lastQuantizedSteps_[instrument]);
					lastQuantizedSteps_[instrument] = currentQuantizedStep;
				}
				player_->playNote(instrument, DrumStep::NORMAL);
			}
		}
	}
}

void StepRecorder::recordSubsteps(unsigned char instrumentIndex, unsigned char subStepStartIndex) {

	unsigned char size = playerSettings_->getDrumInstrumentEventType(instrumentIndex) == PlayerSettings::GATE ?
			playerSettings_->getRecordQuantizationSize() :
			1;
	for (unsigned char subStepIndex = 0; subStepIndex < size; subStepIndex++) {
		unsigned char currentStep = (unsigned char)((subStepIndex + subStepStartIndex) / 4);
		unsigned char currentSubStep = (unsigned char)((subStepIndex + subStepStartIndex) % 4);
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
}

void StepRecorder::stopRecordNote(unsigned char instrumentIndex) {

	if (recordInstrumentStatuses_[instrumentIndex]) {
		recordInstrumentStatuses_[instrumentIndex] = false;
		recordSubsteps(instrumentIndex, lastQuantizedSteps_[instrumentIndex]);
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

