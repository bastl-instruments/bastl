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

#include "Stepper.h"

Stepper::Stepper(IHWLayer * hwLayer) :
	running_(true), stopAfterNextStep_(false), stepCallback_(0), hwLayer_(hwLayer) {
}

void Stepper::update() {
	if (running_) {
		unsigned int elapsedCycles = hwLayer_->getElapsedBastlCycles();

		//Even if the value overflow which happens every ~6 minutes we should get correct value
		//This works in case steps are shorted than 6 minutes which is the rule we suppose we never break

		if ((getBastlCyclesPerStep() != 0) && ((elapsedCycles - lastElapsedCycles_) > getBastlCyclesPerStep())) {
			if (stepCallback_ != 0) {
				#ifdef DEBUG
				printf("\tSending step elapsed cycles: %d\n", elapsedCycles);
				#endif
				stepCallback_();
			}
			if (stopAfterNextStep_) {
				#ifdef DEBUG
				printf("\tStopping\n", elapsedCycles);
				#endif
				stop();
			}

			// catch the next step
			lastElapsedCycles_ += getBastlCyclesPerStep() * ((elapsedCycles - lastElapsedCycles_) % getBastlCyclesPerStep());
		}
	}
}

void Stepper::sync() {
	if (stepCallback_ != 0) {
		stepCallback_();
	}
	lastElapsedCycles_ = hwLayer_->getElapsedBastlCycles();
}

void Stepper::setBPM(unsigned int bpm) {
	if (bpm < 5) {bpm = 5;};
	// Rounding instead of casting could be nice here but lets make it little out of sync
	// One beat is actually 16 bastl steps
	bastlCyclesPerStep_ = (int)((((float)hwLayer_->getBastlCyclesPerSecond()) / ((float)bpm / 60.0)) / 16.0);
}

unsigned char Stepper::getBastlCyclesPerStep() {
	return bastlCyclesPerStep_;
}





