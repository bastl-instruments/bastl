/*
 * LFORandom.h
 *
 *  Created on: 07.04.2015
 *      Author: user
 */

#ifndef LFORANDOM_H_
#define LFORANDOM_H_

#include "LFO.h"
#include "movingAverage.h"
#include "FiFoBuffer.h"

class LFORandom : public LFO {
public:
	LFORandom() {};
	virtual ~LFORandom() {}

	void setSmoothness(uint8_t val);

	virtual void step();
	virtual uint8_t calcOutput();

private:

	void randomStep();


	static const uint8_t bufferSize = 32;

	void addToBuffer(uint16_t val);
	uint16_t getBufferAverage();

	uint16_t buffer[bufferSize];
	uint8_t bufferPos=0;

	uint8_t smoothness;

	uint16_t thisStepDetailed;
	uint16_t nextStepDetailed;


};



#endif /* LFORANDOM_H_ */
