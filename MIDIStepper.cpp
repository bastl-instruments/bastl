/*
 * MIDIStepper.cpp
 *
 *  Created on: Jul 16, 2014
 *      Author: bastl
 */

//#define DEBUG
#ifdef DEBUG

#include <iostream>

#endif

#include "MIDIStepper.h"

MIDIStepper::MIDIStepper(IHWLayer * hwLayer) : Stepper(hwLayer) {
}

unsigned char MIDIStepper::getBastlCyclesPerStep() {
	return nextMidiStepCycles_;
}

void MIDIStepper::stepMIDI() {

	int currentCycles = hwLayer_->getElapsedBastlCycles();
	nextMidiStepCycles_ = (currentCycles - lastMidiStepCycles_) / 2;
	#ifdef DEBUG
	printf("\tCurrentCycles %d LastCycles %d NextMidiCycles %d\n",currentCycles, lastMidiStepCycles_, nextMidiStepCycles_);
	#endif
	lastMidiStepCycles_ = currentCycles;
	sync();
	start();
	stopAfterNextStep();
}

