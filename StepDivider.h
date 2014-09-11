/*
 * StepDevider.h
 *
 *  Created on: Sep 4, 2014
 *      Author: bastl
 */

#ifndef STEPDEVIDER_H_
#define STEPDEVIDER_H_

#include "BastlStepper.h"

class StepDivider : public BastlStepper {
public:
	StepDivider();
	virtual void doStep();
	virtual void update(){}
	void setDivision(unsigned char division);
protected:
	unsigned char numberOfIgnoredSteps_;
	unsigned int lastStepBastlCycles_;
	unsigned char division_;
};

inline void StepDivider::setDivision(unsigned char division) {
	division_ = division;
}
#endif /* STEPDEVIDER_H_ */
