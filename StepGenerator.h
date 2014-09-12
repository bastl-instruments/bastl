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
	virtual void doStep(unsigned int elapsedTimeUnits);
	virtual void update(unsigned int elapsedTimeUnits);
	void setTimeUnitsPerStep(unsigned char timeUnits);
private:
	unsigned int lastStepTimeUnits_;
};

inline void StepGenerator::setTimeUnitsPerStep(unsigned char timeUnits) {
	timeUnitsPerStep_ = timeUnits;
}

#endif /* STEPGENERATOR_H_ */
