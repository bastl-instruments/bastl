/*
 * Tapper.h
 *
 *  Created on: Sep 5, 2014
 *      Author: bastl
 */

#ifndef TAPPER_H_
#define TAPPER_H_

#include "basic.h"
#include "ITapper.h"




class Tapper {
public:
	Tapper();
	~Tapper();

	// Initialize class.
	// maxStepLengthInTimeUnits defines the the slowest tap tempo that is tracked
	// maxRelativeDeviation define how easily a tap cycle is restarted
	void init(uint16_t maxStepLengthInTimeUnits, uint8_t maxRelativeDeviation=20);

	// tap it!
	void tap(uint16_t tapTime);

	// When the tap cycle gets reset, this callback is called
	void setResetCallback(void (*resetCallback)(uint16_t));


	void setStepsPerTap(uint8_t stepsPerTap);
	uint16_t getTimeUnitsPerStep();
	void setStepCallBack(void (*makeStep)());
	bool anyStepDetected();

private:

	static const uint8_t historyLen = 4;
	uint16_t history[4];
	uint8_t historyHead;
	uint8_t historyFillCount;
	void addToHistory(uint16_t val);
	uint16_t getAverage();
	uint16_t getLast();

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
	return (getAverage() + stepsPerTap_/2) / stepsPerTap_;
}

inline void Tapper::setResetCallback(void (*ptr)(uint16_t))  {
	resetCallback = ptr;
}

inline void Tapper::setStepCallBack(void (*makeStep)()) {
	makeStep_ = makeStep;
}

inline bool Tapper::anyStepDetected() {
	return historyFillCount > 0;
}

#endif /* TAPPER_H_ */
