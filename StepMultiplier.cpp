/*
 * StepMultiplier.cpp
 *
 *  Created on: Sep 4, 2014
 *      Author: bastl
 */

#include <StepMultiplier.h>

StepMultiplier::StepMultiplier() : multiplication_(2), stepsLeftToTrigger_(0), lastSubStepBastlCycles_(0), stepBufferCount_(0) , minTriggerTime_(0){
}

void StepMultiplier::init(IHWLayer * hw) {
	BastlStepper::init(hw);
	bastlCyclesPerStep_ = (hw_->getBastlCyclesPerSecond() * 60) / 120;
}

void StepMultiplier::doStep() {
	unsigned char currentTime = hw_->getElapsedBastlCycles();
	stepBufferCount_ += stepsLeftToTrigger_;
	stepsLeftToTrigger_ = (multiplication_ - 1);
	bastlCyclesPerStep_ = currentTime - lastStepBastlCycles_;
	lastStepBastlCycles_ = currentTime;
	lastSubStepBastlCycles_ = currentTime;
	stepBufferCount_++;
	update();
}

void StepMultiplier::update() {
	unsigned char currentTime = hw_->getElapsedBastlCycles();

	// We check if we should add some more steps to the buffer
	if (stepsLeftToTrigger_ != 0) {
		if ((currentTime - lastSubStepBastlCycles_) > (bastlCyclesPerStep_ / multiplication_)) {
			stepBufferCount_++;
			unsigned char subStepsFromLastStep = (multiplication_ - stepsLeftToTrigger_);

			//Next line is handling problems with rounding so instead of increasing the last triggered
			//To the current time we increase it by division of the step multiplied by number of current step
			//That should have little better precision
			lastSubStepBastlCycles_ = lastStepBastlCycles_ + ((bastlCyclesPerStep_ * subStepsFromLastStep) / multiplication_);
			stepsLeftToTrigger_--;

		}
	}

	//And here we trigger steps if there are some and the time from last step is lower then
	//Minimum distance in between steps that could be set by public setter
	if ((stepBufferCount_ > 0) &&
		((currentTime - lastTriggeredStepTime_) > minTriggerTime_)) {
		if (stepCallback_ != 0) {
			stepCallback_();
		}
		lastTriggeredStepTime_ = currentTime;
		stepBufferCount_--;
	}
}
