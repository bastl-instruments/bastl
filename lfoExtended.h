/*
 * lfoExtended.h
 *
 *  Created on: 31.07.2014
 *      Author: user
 */

#ifndef LFOEXTENDED_H_
#define LFOEXTENDED_H_




#include <mozzi_fixmath.h>
#include <mozzi_rand.h>
typedef Q8n8 phaseType;
const uint16_t numbSteps = 256;
const uint8_t numbFBits = 8;

enum LFOBasicWaveform {SAW,TRIANGLE,RANDOM};
enum LFOThresholdType {OVERFLOW,FOLDING};

template <unsigned int UPDATEFREQ>
class lfoExtended {

public:
	void init();

public:
	void setFreq(phaseType freq);
	void setWaveform(LFOBasicWaveform waveform, bool invert = false, bool flop = false, LFOThresholdType type = OVERFLOW);
	void setPhase(uint8_t position);
	void setToStart();
	void setResolution(uint8_t numbStepsToSkip);
	void setThreshold(uint8_t thres);
	void setCurrentValue(uint8_t);

public:
	LFOBasicWaveform getWaveform();
	uint8_t getCurrentValue();
	void step();

	void dumpSettings();


private:
	void incrementPhase();
	//void calcCurrentValue();

	LFOBasicWaveform currentWaveform;
	bool invertWaveform;
	bool flopWaveform;

	phaseType currentPhase;
	uint8_t currentValue;

	uint8_t threshold;
	LFOThresholdType thresholdType;

	phaseType phaseIncrement;

	uint8_t numbStepsToSkip;
	uint8_t lastUnskippedStep;
};



template <unsigned int UPDATEFREQ>
void lfoExtended<UPDATEFREQ>::init() {
	currentWaveform = SAW;
	currentValue = 0;
	numbStepsToSkip = 0;
	threshold = 255;
	currentPhase = 0;
	lastUnskippedStep=0;

	setFreq(0);
}


template <unsigned int UPDATEFREQ>
void lfoExtended<UPDATEFREQ>::setFreq(phaseType freq) {
	phaseIncrement = (((uint32_t)freq*numbSteps))/UPDATEFREQ;
	//Serial.println(phaseIncrement);
}

template <unsigned int UPDATEFREQ>
void lfoExtended<UPDATEFREQ>::setWaveform(LFOBasicWaveform waveform, bool invert, bool flop,LFOThresholdType type) {
	currentWaveform = waveform;
	invertWaveform = invert;
	flopWaveform = flop;
	thresholdType = type;
}

template <unsigned int UPDATEFREQ>
void lfoExtended<UPDATEFREQ>::setPhase(uint8_t pos) {
	currentPhase = pos<<numbFBits;
}

template <unsigned int UPDATEFREQ>
void lfoExtended<UPDATEFREQ>::setToStart() {
	setPhase(0);
	currentValue();
}

template <unsigned int UPDATEFREQ>
void lfoExtended<UPDATEFREQ>::setResolution(uint8_t numbStepsToSkip) {
	this->numbStepsToSkip = numbStepsToSkip;
}

template <unsigned int UPDATEFREQ>
void lfoExtended<UPDATEFREQ>::setThreshold(uint8_t thres) {
	threshold = thres;
}

template <unsigned int UPDATEFREQ>
void lfoExtended<UPDATEFREQ>::setCurrentValue(uint8_t value) {
	currentValue = value;
}

template <unsigned int UPDATEFREQ>
LFOBasicWaveform lfoExtended<UPDATEFREQ>::getWaveform() {
	return currentWaveform;
}

template <unsigned int UPDATEFREQ>
uint8_t lfoExtended<UPDATEFREQ>::getCurrentValue() {

	const uint8_t flopBit = 3;
	const int8_t randomStepWidth = 4;
	int8_t randomCurrentSlopeSum = 0;
	const uint8_t randomCurrentSlopeCount = 5;


	// calculate step from phase
	uint8_t currentIndex = currentPhase>>numbFBits;

	if ((uint8_t)(currentIndex-lastUnskippedStep) < numbStepsToSkip) {
		currentIndex = lastUnskippedStep>>numbFBits;
		//Serial.print("skipped "); Serial.println(currentIndex);
	} else {
		lastUnskippedStep = currentIndex;
	}


	// Render Basic Waveform
	switch(currentWaveform) {
		case SAW: {
			currentValue = currentIndex;
			break;
		}
		case TRIANGLE: {
			if (currentIndex < numbSteps/2) currentValue = currentIndex*2;
			else 				            currentValue = 255-(currentIndex-numbSteps/2)*2;
			break;
		}

		case RANDOM: {

			char max = min(255-currentValue,randomStepWidth);
			char min = max(-currentValue,-randomStepWidth);

			currentValue += rand(min,max);

			break;
		}

		default: break;
	}


	// Invert Waveform
	if (invertWaveform) {
		currentValue = 255-currentValue;
	}

	// Apply Flopping
	if ((flopWaveform) && (currentValue & (1<<flopBit))) {
		currentValue = 0;
	}

	// Apply Overflowing
	if (currentValue > threshold) {

		if (thresholdType == OVERFLOW) currentValue = currentValue % threshold;
		if (thresholdType == FOLDING)  currentValue = threshold - (currentValue % threshold);
	}

	return currentValue;
}

template <unsigned int UPDATEFREQ>
void lfoExtended<UPDATEFREQ>::step() {
	incrementPhase();
	//calcCurrentValue();
}

template <unsigned int UPDATEFREQ>
void lfoExtended<UPDATEFREQ>::incrementPhase() {
	currentPhase += phaseIncrement;


}



template <unsigned int UPDATEFREQ>
void lfoExtended<UPDATEFREQ>::dumpSettings() {

	Serial.print("Basic Waveform: ");
	switch (currentWaveform) {
		case SAW: Serial.println("SAW"); break;
		case TRIANGLE: Serial.println("TRAINGLE"); break;
		case RANDOM: Serial.println("RANDOM"); break;
	}

	Serial.print("Invert: "); Serial.println(invertWaveform);
	Serial.print("Flop: "); Serial.println(flopWaveform);
	Serial.print("Thres: "); Serial.println(threshold);
	Serial.print("Thres type: "); Serial.println(thresholdType);
	Serial.print("Steps to skip: "); Serial.println(numbStepsToSkip);
	Serial.print("Phase Inc: "); Serial.println(phaseIncrement);

}





#endif /* LFOEXTENDED_H_ */
