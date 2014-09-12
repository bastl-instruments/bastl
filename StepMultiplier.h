/*
 * StepMultiplier.h
 *
 *  Created on: Sep 4, 2014
 *      Author: bastl
 */

#ifndef STEPMULTIPLIER_H_
#define STEPMULTIPLIER_H_

#include "BastlStepper.h"

class StepMultiplier : public BastlStepper {
public:
	StepMultiplier();
	virtual void init(unsigned int timeUnitsPerSecond);
	virtual void doStep(unsigned int elapsedTimeUnits);
	virtual void update(unsigned int elapsedTimeUnits);
	void setMultiplication(unsigned char multiplication);
	void setMinTriggerTime(unsigned int miliseconds);
protected:
	unsigned int lastStepTimeUnits_;
	unsigned int lastSubStepTimeUnits_;
	unsigned int lastTriggeredStepTime_;
	unsigned char multiplication_;
	unsigned char stepsLeftToTrigger_;
	unsigned char stepBufferCount_;
	unsigned char minTriggerTimeUnits_;
	bool anyStep_;

};

inline void StepMultiplier::setMultiplication(unsigned char multiplication) {
	multiplication_ = multiplication;
}

// Set the number of timeUnits that needs to be in between two triggered steps.
// Since we loose some precision with integer operator we need to add 1 (+ 1) at the end
// to dont have it shorter for sure
inline void StepMultiplier::setMinTriggerTime(unsigned int timeUnits) {
	minTriggerTimeUnits_ = timeUnits;
}

#endif /* STEPMULTIPLIER_H_ */
