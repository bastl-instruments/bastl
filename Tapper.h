/*
 * Tapper.h
 *
 *  Created on: Sep 5, 2014
 *      Author: bastl
 */

#ifndef TAPPER_H_
#define TAPPER_H_

class Tapper {
public:
	Tapper();
	void init(unsigned int maxStepLengthInTimeUnits = 200, unsigned int  maxStepDeviationInTImeUnits = 50);
	void tap(unsigned int tapTime);
	void setStepsPerTap(unsigned char stepsPerTap);
	unsigned char getTimeUnitsPerStep();
	void setStepCallBack(void (*makeStep)());
private:
	char stepsInRow_;
	unsigned int lastTapTime_;
	unsigned int currentTapTime_;
	unsigned int stepsPerTap_;
	unsigned int maxStepLengthInTimeUnits_;
	unsigned int maxStepDeviationInTImeUnits_;
	void (*makeStep_)();
};

inline void Tapper::setStepsPerTap(unsigned char stepsPerTap) {
	stepsPerTap_ = stepsPerTap;
}

inline unsigned char Tapper::getTimeUnitsPerStep() {
	return currentTapTime_ / stepsPerTap_;
}

inline void Tapper::setStepCallBack(void (*makeStep)()) {
	makeStep_ = makeStep;
}

#endif /* TAPPER_H_ */
