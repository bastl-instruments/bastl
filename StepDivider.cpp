/*
 * StepDevider.cpp
 *
 *  Created on: Sep 4, 2014
 *      Author: bastl
 */

#include "StepDivider.h"

//#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif

StepDivider::StepDivider() : BastlStepper(), numberOfStepsToIgnore_(0), lastStepTimeUnits_(0), division_(0){
}

void StepDivider::doStep(unsigned int elapsedTimeUnits) {
	if (numberOfStepsToIgnore_ == 0) {
		if (stepCallback_ != 0) {
			stepCallback_();
		}
		if (division_ != 0) {
			numberOfStepsToIgnore_ = division_ - 1;
		}
		if (lastStepTimeUnits_ != 0) {
			timeUnitsPerStep_ = elapsedTimeUnits - lastStepTimeUnits_;
		}
		lastStepTimeUnits_ = elapsedTimeUnits;
	} else {

		timeUnitsPerStep_ = ((elapsedTimeUnits - lastStepTimeUnits_) * division_) / (division_ - numberOfStepsToIgnore_);
		numberOfStepsToIgnore_--;
		#ifdef DEBUG
		printf("StepDivider::doStep - step Ignored - TimeUnitsPerStep %d\n", timeUnitsPerStep_);
		#endif
	}
}
