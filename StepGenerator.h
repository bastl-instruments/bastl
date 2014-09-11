/*
 * StepGenerator.h
 *
 *  Created on: Sep 5, 2014
 *      Author: bastl
 */

#ifndef STEPGENERATOR_H_
#define STEPGENERATOR_H_

#include "BastlStepper.h"

class StepGenerator : public BastlStepper {
public:
	StepGenerator();
	virtual void doStep();
	virtual void update();
	void setBastlCyclesPerStep(unsigned char bastlCyclesPerStep);
private:
	unsigned char bastlCyclesPerStep_;
	unsigned char lastStepBastlCycles_;
};

inline void StepGenerator::setBastlCyclesPerStep(unsigned char bastlCyclesPerStep) {
	bastlCyclesPerStep_ = bastlCyclesPerStep;
}

#endif /* STEPGENERATOR_H_ */
