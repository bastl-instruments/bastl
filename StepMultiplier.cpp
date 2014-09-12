/*
 * StepMultiplier.cpp
 *
 *  Created on: Sep 4, 2014
 *      Author: bastl
 */

#include "StepMultiplier.h"

#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif


StepMultiplier::StepMultiplier() : lastSubStepTimeUnits_(0), multiplication_(2), stepsLeftToTrigger_(0), stepBufferCount_(0) , minTriggerTimeUnits_(0), anyStep_(false){
}

void StepMultiplier::init(unsigned int timeUnitsPerSecond) {
	timeUnitsPerStep_ = (timeUnitsPerSecond * 60) / 120;
	#ifdef DEBUG
	printf("StepMultiplier::init - Setting timeUnitsPerStep %d\n", timeUnitsPerStep_);
	#endif
}

void StepMultiplier::doStep(unsigned int elapsedTimeUnits) {

	stepBufferCount_ += stepsLeftToTrigger_;
	stepsLeftToTrigger_ = (multiplication_ - 1);
	if (anyStep_) {
		timeUnitsPerStep_ = elapsedTimeUnits - lastStepTimeUnits_;
	}
	lastStepTimeUnits_ = elapsedTimeUnits;
	lastSubStepTimeUnits_ = elapsedTimeUnits;
	stepBufferCount_++;
	update(elapsedTimeUnits);
	anyStep_ = true;
}

void StepMultiplier::update(unsigned int elapsedTimeUnits) {

	// We check if we should add some more steps to the buffer
	if (stepsLeftToTrigger_ != 0) {
		#ifdef DEBUG
		printf("StepMultiplier::update - Something left to trigger %d\n", stepsLeftToTrigger_);
		printf("StepMultiplier::update - Elapsed : %d LastSubstep: %d Multiplication %d, SubStepLength: %d \n",
				elapsedTimeUnits,
				lastSubStepTimeUnits_,
				multiplication_,
				(timeUnitsPerStep_ / multiplication_));
		#endif
		if ((elapsedTimeUnits - lastSubStepTimeUnits_) > (timeUnitsPerStep_ / multiplication_)) {
			#ifdef DEBUG
			printf("StepMultiplier::update - TRIGGER!\n");
			#endif
			stepBufferCount_++;
			unsigned char subStepsFromLastStep = (multiplication_ - stepsLeftToTrigger_);

			//Next line is handling problems with rounding so instead of increasing the last triggered
			//To the current time we increase it by division of the step multiplied by number of current step
			//That should have little better precision
			lastSubStepTimeUnits_ = lastStepTimeUnits_ + ((timeUnitsPerStep_ * subStepsFromLastStep) / multiplication_);
			stepsLeftToTrigger_--;

		}
	}

	//And here we trigger steps if there are some and the time from last step is lower then
	//Minimum distance in between steps that could be set by public setter
	if ((stepBufferCount_ > 0) &&
		((elapsedTimeUnits - lastTriggeredStepTime_) > minTriggerTimeUnits_)) {
		if (stepCallback_ != 0) {
			stepCallback_();
		}
		lastTriggeredStepTime_ = elapsedTimeUnits;
		stepBufferCount_--;
	}
}
