/*
 * LFOSimple.h
 *
 *  Created on: 07.04.2015
 *      Author: user
 */

#ifndef LFOSIMPLE_H_
#define LFOSIMPLE_H_

#include "LFO.h"

class LFOSimple : public LFO {
public:

	LFOSimple() : waveform(SAW) {
		setNumbStepsToSkip(0);
	}
	virtual ~LFOSimple() {};

	enum Waveform {SAW,TRIANGLE};
	void setWaveform(Waveform);

	void setNumbStepsToSkip(uint8_t stepsToSkip);

	virtual uint8_t calcOutput();

private:

	Waveform waveform;

	uint16_t lastUnskippedPhase;
	uint16_t numbPhaseStepsToSkip;
};



#endif /* LFOSIMPLE_H_ */
