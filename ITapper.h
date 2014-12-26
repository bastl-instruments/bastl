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
	virtual void tap(uint16_t tapTime);
	virtual void setStepsPerTap(uint8_t stepsPerTap);
	virtual uint16_t getTimeUnitsPerStep();
	virtual void setStepCallBack(void (*makeStep)());
	virtual bool anyStepDetected();

};


#endif /* ITAPPER_H_ */
