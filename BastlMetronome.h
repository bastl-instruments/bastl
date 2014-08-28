/*
 * IStepper.h
 *
 *  Created on: Jul 16, 2014
 *      Author: Martin Baar
 */

#ifndef METRONOME_H_
#define METRONOME_H_

#include "IHWLayer.h"

class BastlMetronome {

public:

	BastlMetronome();
	void setStepCallback(void (*stepCallback)());
	void setBPM(unsigned int bpm);
	void update();
	void sync();
	void init(IHWLayer * hwLayer);
	unsigned char getQuantizedStep(unsigned char quantization, bool & played);
	unsigned char getCurrentStepIndex();
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
	enum QuantizedShift {UP, DOWN};

	unsigned int lastElapsedCycles_;
	unsigned int bastlCyclesPerStep_;
	unsigned char currentGlobalStep_;
	unsigned char quatizedStep_;
	QuantizedShift shift_;
};


inline unsigned char BastlMetronome::getCurrentStepIndex() {
	return currentGlobalStep_;
}

inline void BastlMetronome::start() {
	running_ = true;
	lastElapsedCycles_ = hwLayer_->getElapsedBastlCycles();
}

inline void BastlMetronome::stop() {
	running_ = false;
}

inline void BastlMetronome::stopAfterNextStep() {
	stopAfterNextStep_ = true;
}

inline void BastlMetronome::setStepCallback(void (*stepCallback)()) {
	stepCallback_ = stepCallback;
}

#endif /* METRONOME_H_ */
