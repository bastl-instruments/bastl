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
	unsigned int getBPM(unsigned int timeUnitsPerSecond);
	void setStepsPerBeat(unsigned char stepsPerBeat);
	virtual void doStep(unsigned int elapsedTimeUnits) = 0;
	virtual void update(unsigned int elapsedTimeUnits) = 0;
protected:
	unsigned int timeUnitsPerStep_;
	void (*stepCallback_)();
	unsigned char stepsPerBeat_;
};

inline unsigned int BastlStepper::getTimeUnitsPerStep() {
	return timeUnitsPerStep_;
}

inline unsigned int BastlStepper::getBPM(unsigned int timeUnitsPerSecond) {
	return (timeUnitsPerSecond * 60) / (timeUnitsPerStep_ * stepsPerBeat_);
}

inline void BastlStepper::setStepCallback(void (*stepCallback)()) {
	stepCallback_ = stepCallback;
}

inline void BastlStepper::setStepsPerBeat(unsigned char stepsPerBeat) {
	stepsPerBeat_ = stepsPerBeat;
}
#endif /* BASTLSTEPPER_H_ */
