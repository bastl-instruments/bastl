/*
 * LFOSimple.cpp
 *
 *  Created on: 07.04.2015
 *      Author: user
 */


#include "LFOSimple.h"


void LFOSimple::setWaveform(Waveform val) {
	waveform = val;
}


void LFOSimple::setNumbStepsToSkip(uint8_t stepsToSkip) {

	uint16_t val = 257-stepsToSkip;

	numbPhaseStepsToSkip = uint32_t(65536)/val;
}



uint8_t LFOSimple::calcOutput() {

	// check if step will be skipped due to resolution
	uint16_t phaseStepsSinceLast = currentPhase-lastUnskippedPhase;


	// Apply Flopping
	if ((currentStep & flopVector) != 0) {
		return 0;
	}

	// check if resolution is taking affect
	if (phaseStepsSinceLast < numbPhaseStepsToSkip) {
		return currentOutput;
	} else {
		lastUnskippedPhase += numbPhaseStepsToSkip;
	}


	// Calculate Basic Waveform
	switch (waveform) {
	case SAW:
		currentOutput = currentStep;
		break;
	case TRIANGLE: {
		if (currentStep < 128) currentOutput = currentStep*2;
		else 				   currentOutput = 255-((currentStep*2)-255);
		break;
		}
	}

	// Apply Overflowing
	if (currentOutput > thres) {
		if (isFolding) {
			uint8_t sectorNumber = currentOutput/thres;
			if (sectorNumber & 1) currentOutput = thres - (currentOutput % thres);
			else currentOutput = currentOutput % thres;
		} else {
			currentOutput = currentOutput % thres;
		}
	}

	// apply XOR Bits
	currentOutput ^= xorVector;

	return currentOutput;

}
