/*
 * StepSynchronizer.h
 *
 *  Created on: Sep 12, 2014
 *      Author: bastl
 */

#ifndef STEPSYNCHRONIZER_H_
#define STEPSYNCHRONIZER_H_

class StepSynchronizer {
public:
	StepSynchronizer();
	void setCycleLength(unsigned int cycleLength);
	void doStep();
	void reset();
	unsigned char getCurrentStepNumber();
private:
	unsigned int cycleLength_;
	unsigned char currentStepNumber_;
	bool waiting_;
};

inline void StepSynchronizer::setCycleLength(unsigned int cycleLength) {
	cycleLength_ = cycleLength;
}

inline void StepSynchronizer::doStep() {
	if (waiting_) {
		waiting_ = false;
	} else {
		currentStepNumber_ = (currentStepNumber_ + 1) % cycleLength_;
	}
}

inline unsigned char StepSynchronizer::getCurrentStepNumber() {
	return currentStepNumber_;
}

inline void StepSynchronizer::reset() {
	currentStepNumber_ = 0;
	waiting_ = true;
}

#endif /* STEPSYNCHRONIZER_H_ */
