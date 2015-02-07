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
	void init();


public:
	// set the base frequency of the LFO by its period length
	void setBastlCyclesPerPeriod(uint16_t bastlCyclesPerPeriod);

	// associate a step number with a timestamp
	// this acts like a phase shift
	void setToStep(uint8_t stepNumber,uint8_t timestamp);

	// set the basic waveform
	void setWaveform(LFOBasicWaveform waveform);

	// the output value is XORed with xorBits
	void setXOR(uint8_t xorBits);

	// set the output to zero of any of the flopBits are set in the current step number
	void setFlop(uint8_t flopBits);

	// the output value stays constant for numbStepsToSkip steps
	void setResolution(uint8_t numbStepsToSkip);

	// when the output reaches zero or thres, it is either folded or oveflows
	void setThreshold(uint8_t thres, LFOThresholdType type = FOLDING);


public:
	// get the output value associated with the timstamp
	uint8_t getValue(uint16_t timestamp);


private:

	// Waveform
	LFOBasicWaveform currentWaveform;
	uint8_t xorBits;
	uint8_t flopBits;

	// Threshold
	uint8_t threshold;
	LFOThresholdType thresholdType;

	// Timing
	uint8_t currentStep;
	uint8_t lastStep;
	uint16_t timestampOffset;
	uint16_t bastlCyclesPerPeriod;
	uint16_t lastTimestamp;

	uint8_t numbStepsToSkip;
	uint8_t lastUnskippedStep;

	uint8_t currentOutput;




	// parameters
	static const uint8_t maxAbsoluteSlope = 3;
	static const uint8_t maxStepsBetweenSlopeChange = 30;
	static const uint8_t minStepsBetweenSlopeChange = maxStepsBetweenSlopeChange/2;

	// random
	int8_t currentSlope;
	int16_t stepsUntilNextSlopePick;

};









#endif /* LFOEXTENDED_H_ */
