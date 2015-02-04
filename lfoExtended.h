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
	void init();

public:
	void setBastlCyclesPerPeriod(uint16_t bastlCyclesPerPeriod);
	void setToStep(uint8_t stepNumber,uint8_t timestamp);

	void setWaveform(LFOBasicWaveform waveform, bool invert = false, bool flop = false, LFOThresholdType type = FOLDING);
	void setResolution(uint8_t numbStepsToSkip);
	void setThreshold(uint8_t thres);



public:
	uint8_t getValue(uint16_t timestamp);


private:

	// Waveform
	LFOBasicWaveform currentWaveform;
	bool invertWaveform;
	bool flopWaveform;
	uint8_t threshold;
	LFOThresholdType thresholdType;

	uint8_t currentStep;
	uint8_t lastStep;

	uint16_t timestampOffset;
	uint16_t bastlCyclesPerPeriod;

	uint8_t currentOutput;

	uint8_t numbStepsToSkip;
	uint8_t lastUnskippedStep;

	// parameters
	static const uint8_t maxAbsoluteSlope = 3;
	static const uint8_t maxStepsBetweenSlopeChange = 30;
	static const uint8_t minStepsBetweenSlopeChange = maxStepsBetweenSlopeChange/2;

	static const uint8_t flopBit = 3;

	// random
	int8_t currentSlope;
	int16_t stepsUntilNextSlopePick;

};









#endif /* LFOEXTENDED_H_ */
