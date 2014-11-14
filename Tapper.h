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

class Tapper {
public:
	Tapper();
	~Tapper();
	void init(uint16_t maxStepLengthInTimeUnits, uint8_t averageWidth = 4, uint8_t maxRelativeDeviation=20);
	void tap(uint16_t tapTime);
	void setStepsPerTap(uint8_t stepsPerTap);
	uint16_t getTimeUnitsPerStep();
	void setStepCallBack(void (*makeStep)());

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

#endif /* TAPPER_H_ */
