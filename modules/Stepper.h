/*
 * IStepper.h
 *
 *  Created on: Jul 16, 2014
 *      Author: Martin Baar
 */

#ifndef STEPPER_H_
#define STEPPER_H_

#include "../hw/IHWLayer.h"

class Stepper {
public:
	Stepper(IHWLayer * hwLayer);
	void setStepCallback(void (*stepCallback)());
	void setBPM(unsigned int bpm);
	void update();
	void sync();
protected:
	bool running_;
	bool stopAfterNextStep_;
	void (*stepCallback_)();
	IHWLayer * hwLayer_;
	virtual unsigned char getBastlCyclesPerStep();
	void start();
	void stop();
	void stopAfterNextStep();
private:
	unsigned int lastElapsedCycles_;
	unsigned int bastlCyclesPerStep_;
};

inline void Stepper::start() {
	running_ = true;
	lastElapsedCycles_ = hwLayer_->getElapsedBastlCycles();
}

inline void Stepper::stop() {
	running_ = false;
}

inline void Stepper::stopAfterNextStep() {
	stopAfterNextStep_ = true;
}

inline void Stepper::setStepCallback(void (*stepCallback)()) {
	stepCallback_ = stepCallback;
}

#endif /* ISTEPPER_H_ */
