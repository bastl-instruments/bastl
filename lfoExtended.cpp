/*
 * lfoExtended.cpp
 *
 *  Created on: 30.01.2015
 *      Author: user
 */


#include "lfoExtended.h"

void lfoExtended::init() {
	currentWaveform = SAW;
	//currentValue = 0;
	numbStepsToSkip = 0;
	threshold = 255;
	currentStep = 0;
	lastUnskippedStep=0;
	timestampOffset=0;
	currentSlope = 0;

}

void lfoExtended::setBastlCyclesPerPeriod(uint16_t bastlCyclesPerPeriod) {
	this->bastlCyclesPerPeriod = bastlCyclesPerPeriod;
}

/*void lfoExtended::step(uint16_t timestamp) {
	currentStep += ((uint16_t)(timestamp - lastTimestamp)<<8) / (bastlCyclesPerPeriod);
	#ifdef TESTING
	//printf("now %u, before %u, pos %u\n",timestamp,lastTimestamp,currentPosition);
	#endif

	bastlCyclesToNextSlopePick -= (timestamp - lastTimestamp);

	lastTimestamp = timestamp;


}*/

void lfoExtended::setWaveform(LFOBasicWaveform waveform, bool invert, bool flop,LFOThresholdType type) {
	currentWaveform = waveform;
	invertWaveform = invert;
	flopWaveform = flop;
	thresholdType = type;
}

void lfoExtended::setResolution(uint8_t numbStepsToSkip) {
	this->numbStepsToSkip = numbStepsToSkip;
}

void lfoExtended::setThreshold(uint8_t thres) {
	threshold = thres;
}


void lfoExtended::setToStep(uint8_t step, uint8_t timestamp) {
	timestampOffset = timestamp - ((step*bastlCyclesPerPeriod)>>8);
}






uint8_t lfoExtended::getValue(uint16_t timestamp) {

	// calculate the current step of LFO waveform
	uint8_t currentStep = (((timestamp - timestampOffset)<<8) / bastlCyclesPerPeriod) & 0xFF;

	uint8_t stepsSinceLast = currentStep-lastUnskippedStep;

	// check if step will be skipped due to resolution
	if ((uint8_t)(currentStep-lastUnskippedStep) <= numbStepsToSkip) {
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

		//printf("Slope: %i\n",currentSlope);


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

		// Invert Waveform
		if (invertWaveform) {
			currentOutput = 255-currentOutput;
		}




	}

	// Apply Overflowing
	if (currentOutput > threshold) {
		if (thresholdType == OVERFLOW) currentOutput = currentOutput % threshold;
		//if (thresholdType == FOLDING)  currentOutput = threshold - (currentOutput % threshold);
	}


	// Apply Flopping
	if ((flopWaveform) && (currentOutput & (1<<flopBit))) {
		return 0;
	}

	return currentOutput;
}



