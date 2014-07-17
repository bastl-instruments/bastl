/*
 * SteppingHWLayerMock.h
 *
 *  Created on: Jul 16, 2014
 *      Author: bastl
 */

#ifndef STEPPINGHWLAYERMOCK_H_
#define STEPPINGHWLAYERMOCK_H_

#include "../../hw/IHWLayer.h"

class SteppingHWLayerMock : public IHWLayer{
public:
	virtual ButtonState getButtonState(unsigned char index) {return IHWLayer::UP;}
	virtual void setLED(unsigned char index, LedState state) {}
	virtual unsigned char getKnobValue(unsigned char index) {return 0;}
	virtual unsigned char readSRAM(long address) {return 0;}
	virtual void readSRAM(long address, unsigned char * data, unsigned int size) {}
	virtual void writeSRAM(long address, unsigned char data) {}
	virtual void writeSRAM(long address, unsigned char * data, unsigned int size) {}
	virtual unsigned int getElapsedBastlCycles() {return elapsedBastlCycles_;}
	virtual unsigned char getBastlCyclesPerSecond() {return bastlCyclesPerSecond_;}
	void setBastlCyclesPerSecond(unsigned char bastlCyclesPerSecond);
	void setElapsedBastlCycles(unsigned int elapsedBastlCycles);
private:
	unsigned char bastlCyclesPerSecond_;
	unsigned int elapsedBastlCycles_;
};

#endif /* STEPPINGHWLAYERMOCK_H_ */
