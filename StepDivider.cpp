/*
 * StepDevider.cpp
 *
 *  Created on: Sep 4, 2014
 *      Author: bastl
 */

#include <StepDivider.h>

StepDivider::StepDivider() : BastlStepper(), numberOfIgnoredSteps_(0), lastStepBastlCycles_(0), division_(0){
}

void StepDivider::doStep() {
	if (numberOfIgnoredSteps_ == 0) {
		if (stepCallback_ != 0) {
			stepCallback_();
		}
		if (division_ != 0) {
			numberOfIgnoredSteps_ = numberOfIgnoredSteps_ + 1 % division_;
		}
		unsigned int currentTime = hw_->getElapsedBastlCycles();
		if (lastStepBastlCycles_ != 0) {
			bastlCyclesPerStep_ = currentTime - lastStepBastlCycles_;
		}
		lastStepBastlCycles_ = currentTime;
	}
}
