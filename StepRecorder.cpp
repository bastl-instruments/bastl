/*
 * StepRecorder.cpp
 *
 *  Created on: Aug 28, 2014
 *      Author: bastl
 */

#include "StepRecorder.h"

StepRecorder::StepRecorder() : player_(0), memory_(0), playerSettings_(0), record_(true) {
}

void StepRecorder::init(Player* player, IStepMemory * memory, PlayerSettings* playerSettings) {
	player_ = player;
	memory_ = memory;
	playerSettings_ = playerSettings;
}


void StepRecorder::recordNote(unsigned char instrumentIndex) {
	unsigned char currentStep = player_->getCurrentInstrumentStep(instrumentIndex);
	unsigned char currentSubStep = player_->getCurrentInstrumentSubStep(instrumentIndex);
	DrumStep step = memory_->getDrumStep(instrumentIndex, 0, currentStep);

	if (step.isMuted() || step.getSubStep(currentSubStep) == DrumStep::OFF) {
		if (step.isMuted()) {
			step.setMuted(false);
			for (unsigned char subStep = 0; subStep < 4; subStep++) {
				step.setSubStep(subStep, DrumStep::OFF);
			}
		}
		step.setSubStep(currentSubStep, DrumStep::NORMAL);
		memory_->setDrumStep(instrumentIndex, 0, currentStep, step);
		player_->playNote(instrumentIndex, DrumStep::NORMAL);
	}
}

void StepRecorder::recordMIDINote(unsigned char channel, unsigned char note) {
	unsigned char instrumentID = 0;
	bool instrumentMapped = playerSettings_->getDrumInstrumentIndexFromMIDIMessage(channel, note, instrumentID);
	if (instrumentMapped) {
		recordNote(instrumentID);
	}
}

#include <StepRecorder.h>

