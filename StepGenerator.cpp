/*
 * StepGenerator.cpp
 *
 *  Created on: Sep 5, 2014
 *      Author: bastl
 */

#include "StepGenerator.h"

#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif

StepGenerator::StepGenerator() : BastlStepper(), lastStepTimeUnits_(0){
}

void StepGenerator::doStep(unsigned int elapsedTimeUnits) {

	//This is kind of sync method
	#ifdef DEBUG
	printf("StepGenerator::doStep - timeElapsedFromLast: %d , timeUnitsPerStep,: %d\n",
			(elapsedTimeUnits - lastStepTimeUnits_),
			timeUnitsPerStep_);
	#endif
	if (elapsedTimeUnits - lastStepTimeUnits_  < (timeUnitsPerStep_ / 3)) {
		lastStepTimeUnits_ = elapsedTimeUnits;
		#ifdef DEBUG
		printf("StepGenerator::doStep - Move this little forward! to: %d\n", lastStepTimeUnits_);
		#endif
	} else {
		lastStepTimeUnits_ = (elapsedTimeUnits - timeUnitsPerStep_) - 1;
		#ifdef DEBUG
		printf("StepGenerator::doStep - Move this little backward! to: %d\n", lastStepTimeUnits_);
		#endif
	}
	update(elapsedTimeUnits);
}

void StepGenerator::update(unsigned int elapsedTimeUnits) {
	if (elapsedTimeUnits - lastStepTimeUnits_ > timeUnitsPerStep_) {
		lastStepTimeUnits_ = elapsedTimeUnits - ((elapsedTimeUnits - lastStepTimeUnits_) % timeUnitsPerStep_);
		#ifdef DEBUG
		printf("StepGenerator::update - TRIGGER! elapsed: %d, timeUnitsPerStep: %d, currentStep %d\n", elapsedTimeUnits, timeUnitsPerStep_, lastStepTimeUnits_);
		#endif
		if (stepCallback_ != 0) {
			stepCallback_();
		}
	}
}
