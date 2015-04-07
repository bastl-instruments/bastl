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

uint8_t LFOSimple::calcOutput() {

	// Apply Flopping
	if ((currentStep & flopVector) != 0) {
		return 0;
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
