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
	void setBPM(unsigned int bpm, unsigned int timeUnitsPerSecond);
	void update(unsigned int elapsedTimeUnits);
	void sync();
	unsigned char getQuantizedStep(unsigned char quantization, bool & played);
	unsigned char getCurrentStepIndex();
protected:
	bool running_;
	bool stopAfterNextStep_;
	void (*stepCallback_)();
	virtual unsigned char getTimeUnitsPerStep();
	void start(unsigned int elapsedTimeUnits);
	void stop();
	void stopAfterNextStep();
private:
	enum QuantizedShift {UP, DOWN};

	unsigned int lastElapsedTimeUnits_;
	unsigned int timeUnitsPerStep_;
	unsigned char currentGlobalStep_;
	unsigned char quatizedStep_;
	QuantizedShift shift_;
};


inline unsigned char BastlMetronome::getCurrentStepIndex() {
	return currentGlobalStep_;
}

inline void BastlMetronome::start(unsigned int elapsedTimeUnits) {
	running_ = true;
	lastElapsedTimeUnits_ = elapsedTimeUnits;
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
