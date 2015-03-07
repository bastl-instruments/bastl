/*
 * lfoExtended.h
 *
 *  Created on: 31.07.2014
 *      Author: user
 */

#ifndef LFOEXTENDED_H_
#define LFOEXTENDED_H_


#include "basic.h"
#include "random.h"

#ifdef TESTING
#include <stdio.h>
#endif


enum LFOBasicWaveform {SAW,TRIANGLE,RANDOM};
enum LFOThresholdType {OVERFLOW,FOLDING};



class lfoExtended {

public:
	// Initialize all settings
	void init(uint16_t bastlCycleFrequency);


public:
	// set the base frequency of the LFO by its period length
	void setBastlCyclesPerPeriod(uint16_t bastlCyclesPerPeriod);

	// set to given step number
	void setToStep(uint8_t stepNumber);

	// set the basic waveform
	void setWaveform(LFOBasicWaveform waveform);

	// the output value is XORed with xorBits
	void setXOR(uint8_t xorBits);

	// set the output to zero of any of the flopBits are set in the current step number
	void setFlop(uint8_t flopBits);

	// one period is divided into stepsPerPeriod steps during which the output remains constant
	void setResolution(uint8_t stepsPerPeriod);

	// when the output reaches zero or thres, it is either folded or oveflows
	void setThreshold(uint8_t thres, LFOThresholdType type = FOLDING);

	inline void step() {
		currentPhase += phaseIncrement;
	}


public:
	// get the current output value
	uint8_t getValue();

	// get the current output value but step() before to catch up with current timestamp
	uint8_t getValue(uint16_t timestamp);


private:

	// Waveform
	LFOBasicWaveform currentWaveform;
	uint8_t xorBits;
	uint8_t flopBits;

	// Resolution
	uint16_t numbPhaseStepsToSkip;
	uint16_t lastUnskippedPhase;

	// Threshold
	uint8_t threshold;
	LFOThresholdType thresholdType;

	// Timing
	uint16_t currentPhase;
	uint8_t currentStep;

	uint16_t lastTimeStamp;

	uint16_t bastlCycleFrequency;
	uint16_t phaseIncrement;

	uint8_t currentOutput;


	// Random
	uint8_t maxAbsoluteSlope;
	static const uint8_t maxStepsBetweenSlopeChange = 40;
	int8_t currentSlope;
	int16_t stepsUntilNextSlopePick;

};









#endif /* LFOEXTENDED_H_ */
