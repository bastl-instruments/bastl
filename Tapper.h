/*
 * Tapper.h
 *
 *  Created on: Sep 5, 2014
 *      Author: bastl
 */

#ifndef TAPPER_H_
#define TAPPER_H_

#include <inttypes.h>
#include "movingAverage.h"
#include "ITapper.h"

class Tapper : ITapper {
public:
	Tapper();
	virtual ~Tapper();
	void init(uint16_t maxStepLengthInTimeUnits, uint8_t averageWidth = 4, uint8_t maxRelativeDeviation=20);
	virtual void tap(uint16_t tapTime);
	virtual void setStepsPerTap(uint8_t stepsPerTap);
	virtual uint16_t getTimeUnitsPerStep();
	virtual void setStepCallBack(void (*makeStep)());
	virtual bool anyStepDetected();
private:

	MovingAverageLinear<uint16_t>* history;

	uint16_t lastTapTime;
	uint8_t stepsPerTap_;
	uint16_t maxStepLengthInTimeUnits;
	uint8_t maxRelativeDeviation;
	void (*makeStep_)();
};

inline void Tapper::setStepsPerTap(uint8_t stepsPerTap) {
	stepsPerTap_ = stepsPerTap;
}

inline uint16_t Tapper::getTimeUnitsPerStep() {
	return history->getAverage() / stepsPerTap_;
}

inline void Tapper::setStepCallBack(void (*makeStep)()) {
	makeStep_ = makeStep;
}

inline bool Tapper::anyStepDetected() {
	return history->getFillCount() > 0;
}

#endif /* TAPPER_H_ */
