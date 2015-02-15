/*
 * Tapper.h
 *
 *  Created on: Sep 5, 2014
 *      Author: bastl
 */

#ifndef TAPPER_H_
#define TAPPER_H_

#include "basic.h"
#include "movingAverage.h"
#include "ITapper.h"




class Tapper {
public:
	Tapper();
	~Tapper();

	// Initialize class.
	// maxStepLengthInTimeUnits defines the the slowest tap tempo that is tracked
	// maxRelativeDeviation define how easily a tap cycle is restarted
	void init(uint16_t maxStepLengthInTimeUnits, uint8_t averageWidth = 4, uint8_t maxRelativeDeviation=20);

	// tap it!
	void tap(uint16_t tapTime);

	// When the tap cycle gets reset, this callback is called
	void setResetCallback(void (*resetCallback)(uint16_t));


	void setStepsPerTap(uint8_t stepsPerTap);
	uint16_t getTimeUnitsPerStep();
	void setStepCallBack(void (*makeStep)());
	bool anyStepDetected();

private:

	MovingAverageLinear<uint16_t>* history;

	uint16_t lastTapTime;
	uint8_t stepsPerTap_;
	uint16_t maxStepLengthInTimeUnits;
	uint8_t maxRelativeDeviation;
	uint8_t firstOfCycle;
	void (*resetCallback)(uint16_t);
	void (*makeStep_)();
};

inline void Tapper::setStepsPerTap(uint8_t stepsPerTap) {
	stepsPerTap_ = stepsPerTap;
}

inline uint16_t Tapper::getTimeUnitsPerStep() {
	return history->getAverage() / stepsPerTap_;
}

inline void Tapper::setResetCallback(void (*ptr)(uint16_t))  {
	resetCallback = ptr;
}

inline void Tapper::setStepCallBack(void (*makeStep)()) {
	makeStep_ = makeStep;
}

inline bool Tapper::anyStepDetected() {
	return history->getFillCount() > 0;
}

#endif /* TAPPER_H_ */
