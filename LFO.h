/*
 * LFO.h
 *
 *  Created on: 07.04.2015
 *      Author: user
 */

#ifndef LFO_H_
#define LFO_H_

#include <inttypes.h>


class LFO {
public:
	LFO();
	virtual ~LFO(){};
	void init();
	void setFrequency(uint16_t bastlCyclesPerPeriod);

	void setToStep(uint8_t stepNumber, uint16_t timestamp);

	void setFlop(uint8_t bitVector);
	void setXOR(uint8_t bitVector);
	void setFolding(uint8_t thres);
	void setOverflow(uint8_t thres);

	uint8_t getValue(uint16_t timestamp);

	virtual void step();
	virtual uint8_t calcOutput() = 0;


protected:
	uint16_t currentPhase;
	uint8_t currentStep;
	uint16_t phaseIncrement;

	uint16_t lastTimestamp;

	uint8_t flopVector;
	uint8_t xorVector;
	uint8_t thres;
	bool	isFolding;

	uint8_t currentOutput;

};




#endif /* LFO_H_ */
