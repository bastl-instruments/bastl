/*
 * SimplifiedTapper.h
 *
 *  Created on: Sep 5, 2014
 *      Author: bastl
 */

#ifndef SIMPLIFIED_TAPPER_H_
#define SIMPLIFIED_TAPPER_H_

#include "ITapper.h"

class SimplifiedTapper {
public:
	SimplifiedTapper();
	virtual ~SimplifiedTapper();
	virtual void init(unsigned int maxStepLengthInTimeUnits = 200, unsigned int  maxStepDeviationInTImeUnits = 50);
	virtual void tap(unsigned int tapTime);
	virtual void setStepsPerTap(unsigned char stepsPerTap);
	virtual unsigned char getTimeUnitsPerStep();
	virtual void setStepCallBack(void (*makeStep)());
	virtual bool anyStepDetected();
	unsigned int lastTapTime;
private:
	char stepsInRow_;
	unsigned int lastTapTime_;
	unsigned int currentTapTime_;
	unsigned int stepsPerTap_;
	unsigned int maxStepLengthInTimeUnits_;
	unsigned int maxStepDeviationInTImeUnits_;

	void (*makeStep_)();
};

inline void SimplifiedTapper::setStepsPerTap(unsigned char stepsPerTap) {
	stepsPerTap_ = stepsPerTap;
}

inline unsigned char SimplifiedTapper::getTimeUnitsPerStep() {
	return currentTapTime_ / stepsPerTap_;
}

inline void SimplifiedTapper::setStepCallBack(void (*makeStep)()) {
	makeStep_ = makeStep;
}

inline bool SimplifiedTapper::anyStepDetected() {
	return stepsInRow_ != 0;
}

#endif /* TAPPER_H_ */
