/*
 * BastlStepper.h
 *
 *  Created on: Sep 4, 2014
 *      Author: bastl
 */

#ifndef BASTLSTEPPER_H_
#define BASTLSTEPPER_H_

class BastlStepper {
public:
	BastlStepper();
	void setStepCallback(void (*stepCallback)());
	unsigned int getTimeUnitsPerStep();
	virtual void doStep(unsigned int elapsedTimeUnits) = 0;
	virtual void update(unsigned int elapsedTimeUnits) = 0;
protected:
	unsigned int timeUnitsPerStep_;
	void (*stepCallback_)();
};

inline unsigned int BastlStepper::getTimeUnitsPerStep() {
	return timeUnitsPerStep_;
}

inline void BastlStepper::setStepCallback(void (*stepCallback)()) {
	stepCallback_ = stepCallback;
}

#endif /* BASTLSTEPPER_H_ */
