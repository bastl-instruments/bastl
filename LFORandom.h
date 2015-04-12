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
	LFORandom() :  bufferPos(0), smoothness(0) {};
	virtual ~LFORandom() {}

	void setSmoothness(uint8_t val);

	virtual void step();
	virtual uint8_t calcOutput();

private:

	uint16_t thisStepDetailed;

	void addToBuffer(uint16_t val);
	uint16_t getBufferAverage();
	static const uint8_t bufferSize = 32;
	uint16_t buffer[bufferSize];
	uint8_t bufferPos;

	uint8_t smoothness;


};



#endif /* LFORANDOM_H_ */
