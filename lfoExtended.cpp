/*
 * lfoExtended.cpp
 *
 *  Created on: 30.01.2015
 *      Author: user
 */


#include "lfoExtended.h"

void lfoExtended::init() {
	currentWaveform = SAW;
	numbStepsToSkip = 0;
	threshold = 255;
	thresholdType = FOLDING;
	currentStep = 0;
	lastUnskippedStep=currentStep-128;
	timestampOffset=0;
	lastTimestamp=0;
	currentSlope = 0;
	xorBits = 0;
	flopBits=0;

}

void lfoExtended::setBastlCyclesPerPeriod(uint16_t bastlCyclesPerPeriod) {
	this->bastlCyclesPerPeriod = bastlCyclesPerPeriod;
}

void lfoExtended::setWaveform(LFOBasicWaveform waveform) {
	currentWaveform = waveform;
}
void lfoExtended::setXOR(uint8_t xorBits) {
	this->xorBits = xorBits;
}

void lfoExtended::setFlop(uint8_t flopBits) {
	this->flopBits = flopBits;
}

void lfoExtended::setResolution(uint8_t numbStepsToSkip) {
	this->numbStepsToSkip = numbStepsToSkip;
}

void lfoExtended::setThreshold(uint8_t thres,LFOThresholdType type) {
	threshold = thres;
	thresholdType = type;
}


void lfoExtended::setToStep(uint8_t step, uint8_t timestamp) {
	timestampOffset = timestamp - ((step*bastlCyclesPerPeriod)>>8);
	#ifdef TESTING
	//printf("Offset: %u\n",timestampOffset);
	#endif
}




//#define VERBOSE

uint8_t lfoExtended::getValue(uint16_t timestamp) {

	#ifdef VERBOSE
	printf("%u ",timestamp);
	printf("%u ",lastTimestamp);
	#endif

	if (timestamp <= lastTimestamp) setToStep(currentStep,timestamp);
	lastTimestamp = timestamp;

	// calculate the current step of LFO waveform
	currentStep = ((((uint16_t)(timestamp - timestampOffset)<<8)) / bastlCyclesPerPeriod);

	#ifdef VERBOSE
	printf("%u ",timestampOffset);
	printf("%u ",currentStep);
	#endif


	uint8_t stepsSinceLast = currentStep-lastUnskippedStep;

	// check if step will be skipped due to resolution
	if ((uint8_t)(currentStep-lastUnskippedStep) < numbStepsToSkip) {
		return currentOutput;
	}
	lastUnskippedStep = currentStep;





	if (currentWaveform == RANDOM) {

		// count down the steps that this slope is used
		stepsUntilNextSlopePick -= stepsSinceLast;

		// get new slope and its duration if it's time for it
		if (stepsUntilNextSlopePick <= 0) {
			currentSlope = (bastlRandom::range(0,2*maxAbsoluteSlope+1)-maxAbsoluteSlope);
			stepsUntilNextSlopePick = bastlRandom::range(minStepsBetweenSlopeChange,maxStepsBetweenSlopeChange);
		}

		#ifdef TESTING
		//printf("Slope: %i\n",currentSlope);
		#endif


		// check direction of slope if folding
		if (thresholdType == FOLDING) {
			int16_t tmpCurrentValue = currentOutput+currentSlope;
			if ((tmpCurrentValue > threshold) || (tmpCurrentValue <0)) {
				currentSlope = -currentSlope;
			}
		}

		// add new slope to current output value
		currentOutput += currentSlope;


	} else {

		// render Basic Waveform
		switch(currentWaveform) {

			case SAW: {
				currentOutput = currentStep;
				#ifdef TESTING
				//printf("step: %u -> out: %u",currentStep,currentOutput);
				#endif
				break;
			}

			case TRIANGLE: {
				if (currentStep < 128) currentOutput = currentStep*2;
				else 				   currentOutput = 255-((currentStep*2)-255);
				break;
			}

			default: break;
		}

	}

	// Apply Overflowing
	if (currentOutput > threshold) {
		if (thresholdType == OVERFLOW) {
			currentOutput = currentOutput % threshold;
		} else {
			// FOLDING
			uint8_t sectorNumber = currentOutput/threshold;
			if (sectorNumber & 1) currentOutput = threshold - (currentOutput % threshold);
			else currentOutput = currentOutput % threshold;
		}
	}

	// apply XOR Bits
	currentOutput ^= xorBits;

	// check if flopping is taken affect
	if (currentStep & flopBits) {
		return 0;
	}


	return currentOutput;
}



