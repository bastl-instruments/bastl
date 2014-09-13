/*
 * Stepper.cpp
 *
 *  Created on: Jul 16, 2014
 *      Author: Martin Baar
 */


//#define DEBUG
#ifdef DEBUG

#include <iostream>

#endif

#include "BastlMetronome.h"

BastlMetronome::BastlMetronome():
	running_(true), stopAfterNextStep_(false), stepCallback_(0), currentGlobalStep_(0), quatizedStep_(0), shift_(DOWN) {
}

unsigned char BastlMetronome::getQuantizedStep(unsigned char quantization, bool & played) {
	unsigned char result = quatizedStep_ / quantization;
	result = result + (quatizedStep_ % quantization > quatizedStep_ / 2) ? 1 : 0;
	if (quatizedStep_ % quantization == quantization / 2) {
		if (shift_ == DOWN) {
			result --;
		}
	}
	result *= quantization;
	played = result < currentGlobalStep_;
	return result;
}

void BastlMetronome::update(unsigned int elapsedTimeUnits) {

	quatizedStep_ = currentGlobalStep_;
	shift_ = DOWN;
	if ((elapsedTimeUnits - lastElapsedTimeUnits_) > (getTimeUnitsPerStep() / 2)) {
		quatizedStep_ = (quatizedStep_ + 1) % 256;
		shift_ = UP;
	}
	if (running_) {

		//Even if the value overflow which happens every ~6 minutes we should get correct value
		//This works in case steps are shorted than 6 minutes which is the rule we suppose we never break

		if ((getTimeUnitsPerStep() != 0) && ((elapsedTimeUnits - lastElapsedTimeUnits_) > getTimeUnitsPerStep())) {
			if (stepCallback_ != 0) {
				#ifdef DEBUG
				printf("\tSending step elapsed cycles: %d\n", elapsedCycles);
				#endif
				stepCallback_();
				currentGlobalStep_ = (currentGlobalStep_ + 1) % 256;
			}
			if (stopAfterNextStep_) {
				#ifdef DEBUG
				printf("\tStopping\n", elapsedCycles);
				#endif
				stop();
			}

			// catch the next step
			lastElapsedTimeUnits_ = elapsedTimeUnits; // getBastlCyclesPerStep() * ((elapsedCycles - lastElapsedCycles_) % getBastlCyclesPerStep());
		}
	}
}

void BastlMetronome::sync() {
	if (stepCallback_ != 0) {
		stepCallback_();
		currentGlobalStep_ = (currentGlobalStep_ + 1) % 256;
	}
	//lastElapsedTimeUnits_ = elapsedTimeUnits;
}

void BastlMetronome::setBPM(unsigned int bpm, unsigned int timeUnitsPerSecod) {
	if (bpm < 5) {bpm = 5;};
	// Rounding instead of casting could be nice here but lets make it little out of sync
	// One beat is actually 16 bastl steps

	timeUnitsPerStep_ = ((timeUnitsPerSecod * 60) / bpm)  / 16;
}

unsigned char BastlMetronome::getTimeUnitsPerStep() {
	return timeUnitsPerStep_;
}





