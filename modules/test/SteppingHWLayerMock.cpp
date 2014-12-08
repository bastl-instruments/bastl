/*
 * SteppingHWLayerMock.cpp
 *
 *  Created on: Jul 16, 2014
 *      Author: bastl
 */

#include "SteppingHWLayerMock.h"

void SteppingHWLayerMock::setBastlCyclesPerSecond(unsigned int bastlCyclesPerSecond) {
	bastlCyclesPerSecond_ = bastlCyclesPerSecond;
}

void SteppingHWLayerMock::setElapsedBastlCycles(unsigned int elapsedBastlCycles) {
	elapsedBastlCycles_ = elapsedBastlCycles;
}


