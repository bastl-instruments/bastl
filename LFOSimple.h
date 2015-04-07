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

	enum Waveform {SAW,TRIANGLE};

	LFOSimple() : waveform(SAW) {}
	virtual ~LFOSimple() {};

	void setWaveform(Waveform);

	void setStepsPerPeriod(uint8_t StepsPerPeriod);

	virtual uint8_t calcOutput();

private:

	Waveform waveform;
};



#endif /* LFOSIMPLE_H_ */
