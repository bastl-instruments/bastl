/*
 * ITapper.h
 *
 *  Created on: Dec 26, 2014
 *      Author: martin
 */

#ifndef ITAPPER_H_
#define ITAPPER_H_

#include <inttypes.h>

class ITapper {
public:
	virtual void tap(uint16_t tapTime) = 0;
	virtual void setStepsPerTap(uint8_t stepsPerTap) = 0;
	virtual uint16_t getTimeUnitsPerStep() = 0;
	virtual void setStepCallBack(void (*makeStep)()) = 0;
	virtual bool anyStepDetected() = 0;

};


#endif /* ITAPPER_H_ */
