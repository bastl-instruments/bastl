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
	unsigned char getCurrentStepNumber();
private:
	unsigned int cycleLength_;
	unsigned char currentStepNumber_;
};

inline void StepSynchronizer::setCycleLength(unsigned int cycleLength) {
	cycleLength_ = cycleLength;
}

inline void StepSynchronizer::doStep() {
	currentStepNumber_ = (currentStepNumber_ + 1) % cycleLength_;
}

inline unsigned char StepSynchronizer::getCurrentStepNumber() {
	return currentStepNumber_;
}

#endif /* STEPSYNCHRONIZER_H_ */
