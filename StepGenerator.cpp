/*
 * StepGenerator.cpp
 *
 *  Created on: Sep 5, 2014
 *      Author: bastl
 */

#include <StepGenerator.h>


StepGenerator::StepGenerator() : BastlStepper(), bastlCyclesPerStep_(10), lastStepBastlCycles_(0){
}

void StepGenerator::doStep() {

	//This is kind of sync method
	unsigned int currentTime = hw_->getElapsedBastlCycles();
	if (currentTime - lastStepBastlCycles_  < (bastlCyclesPerStep_ / 10)) {
		lastStepBastlCycles_ = currentTime;
	} else if ((lastStepBastlCycles_ + bastlCyclesPerStep_ - currentTime) < (bastlCyclesPerStep_ / 10)) {
		lastStepBastlCycles_ -= (lastStepBastlCycles_ + bastlCyclesPerStep_ - currentTime);
	}
	update();
}

void StepGenerator::update() {
	unsigned int currentTime = hw_->getElapsedBastlCycles();
	if (currentTime - lastStepBastlCycles_ > bastlCyclesPerStep_) {
		lastStepBastlCycles_ += bastlCyclesPerStep_;
		if (stepCallback_ != 0) {
			stepCallback_();
		}
	}
}
