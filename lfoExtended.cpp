/*
 * lfoExtended.cpp
 *
 *  Created on: 30.01.2015
 *      Author: user
 */

#include "lfoExtended.h"
#include "random.h"



//#define VERBOSE

void lfoExtended::init(uint16_t bastlCycleFrequency) {
	currentWaveform = SAW;
	numbPhaseStepsToSkip = 0;
	threshold = 255;
	thresholdType = FOLDING;
	currentStep = 0;
	lastUnskippedPhase = 0;
	currentSlope = 0;
	xorBits = 0;
	flopBits=0;
	currentPhase = 0;
	lastTimeStamp = 0;

	this->bastlCycleFrequency = bastlCycleFrequency;

}

void lfoExtended::setBastlCyclesPerPeriod(uint16_t bastlCyclesPerPeriod) {
	if (bastlCyclesPerPeriod==0) {
		phaseIncrement = 0;
		return;
	}

	phaseIncrement = ((uint32_t)65536 + (bastlCyclesPerPeriod>>1))/bastlCyclesPerPeriod;
	maxAbsoluteSlope = ((uint16_t)phaseIncrement+100)>>6;
	#ifdef VERBOSE
	printf("Cycles per Period %u = phaseincrement of %u\n",bastlCyclesPerPeriod,phaseIncrement);
	printf("Random max Steps betweene slope change: %u \n\n",maxStepsBetweenSlopeChange);
	#endif
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

void lfoExtended::setResolution(uint8_t stepsPerPeriod) {
	if (stepsPerPeriod == 0) stepsPerPeriod = 1; // preventing division by zero

	this->numbPhaseStepsToSkip = ((uint32_t)65536/(stepsPerPeriod)-2);
	#ifdef VERBOSE
		printf("\nPhase steps to skip %u\n",numbPhaseStepsToSkip);
	#endif
}

void lfoExtended::setThreshold(uint8_t thres,LFOThresholdType type) {
	threshold = thres;
	thresholdType = type;
}

void lfoExtended::setToStep(uint8_t step) {
	currentPhase =  step << 8;
}


uint8_t lfoExtended::getValue(uint16_t timestamp) {

	while(lastTimeStamp!=timestamp) {
		step();
		lastTimeStamp++;
	}

	return getValue();
}



uint8_t lfoExtended::getValue() {

	#ifdef VERBOSE
	printf("Getting output for %u\n",currentPhase);
	#endif

	// check if step will be skipped due to resolution
	uint16_t phaseStepsSinceLast = currentPhase-lastUnskippedPhase;

	#ifdef VERBOSE
	printf("Phase Steps since last Call: %u\n",phaseStepsSinceLast);
	#endif

	// Break down current Phase to current Step //
	currentStep = currentPhase >> 8;


	// check if flopping is taking affect
	if ((currentStep & flopBits)) {
		return 0;
	}

	// check if resolution is taking affect
	if (phaseStepsSinceLast < numbPhaseStepsToSkip) {
		return currentOutput;
	} else {
		lastUnskippedPhase += numbPhaseStepsToSkip;
	}


	// check how many whole steps have passed to decide if new random slope must be chosen
	uint8_t stepsSinceLast = currentStep - lastStep;
	if (stepsSinceLast) lastStep = currentStep;




	if (currentWaveform == RANDOM) {

		// count down the steps that this slope is used
		stepsUntilNextSlopePick -= stepsSinceLast;

		// get new slope and its duration if it's time for it
		if (stepsUntilNextSlopePick <= 0) {
			currentSlope = bastlRandom::range(0,(2*maxAbsoluteSlope))-maxAbsoluteSlope;
			stepsUntilNextSlopePick = bastlRandom::range(20/2,40);
			#ifdef VERBOSE
				printf("\nPick: %i for %i\n",currentSlope,stepsUntilNextSlopePick);
			#endif
		}


		// check direction of slope if folding
		if (thresholdType == FOLDING) {
			int16_t tmpCurrentValue = currentOutput+currentSlope;//*stepsSinceLast;
			if ((tmpCurrentValue > threshold) || (tmpCurrentValue <0)) {
				currentSlope = -currentSlope;
			}
		}

		// add new slope to current output value
		currentOutput += currentSlope;//*stepsSinceLast;


	} else {

		// render Basic Waveform
		switch(currentWaveform) {

			case SAW: {
				currentOutput = currentStep;
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



	return currentOutput;
}



