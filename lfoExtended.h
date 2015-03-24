/*
 * lfoExtended.h
 *
 *  Created on: 31.07.2014
 *      Author: user
 */

#ifndef LFOEXTENDED_H_
#define LFOEXTENDED_H_


#include "basic.h"


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
	// due to fixed point precision, the actual waveform frequency will be:
	// f_out = f_in * floor(floor(2^16 + p/2)/p) / 2^16 ~ f_in/p
	// minimum period: 2
	// maximum period: 65535
	void setBastlCyclesPerPeriod(uint16_t bastlCyclesPerPeriod);

	// set waveform to given step number at a given time
	void setToStep(uint8_t stepNumber, uint16_t time);

	// set the shape basic waveform
	// TRIANGLE, SAW or RANDOM
	void setWaveform(LFOBasicWaveform waveform);

	// the output value is XORed with xorBits
	void setXOR(uint8_t xorBits);

	// set the output to zero if any of the flopBits are set in the current step number
	// to create less advanced patterns, set flopBits to (1<<n), giving you 2^(7-n) flops per period
	void setFlop(uint8_t flopBits);

	// one period is divided into stepsPerPeriod steps during which the output remains constant
	// TODO: value of 1 is not working as expected
	void setResolution(uint8_t stepsPerPeriod);

	// when the output reaches zero or thres, it is either folded or oveflows
	void setThreshold(uint8_t thres, LFOThresholdType type = FOLDING);

	// if you call this function at a frequency f, you will render a waveform of a fixed frequency ~ f/period
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
	uint8_t lastStep;

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
