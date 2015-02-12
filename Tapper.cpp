/*
 * Tapper.cpp
 *
 *  Created on: Sep 5, 2014
 *      Author: bastl
 */

#include "Tapper.h"

//#define TESTING
#ifdef TESTING
#include <stdio.h>
#endif





/* Functionality explained
 *
 * # A tap cycle consists of a number of taps that occur in approximately
 *   equal distances
 * # Average is taken over the last *averageWidth* tap time DIFFERENCES to calculate tap tempo
 * # An active cycle is reset when the relative deviation exceeds a given limit.
 *   This is considered as a desired tempo change
 * # Also absolute limits are needed when no tempo is known yet
 *
 */


Tapper::Tapper() :		history(0),
						lastTapTime(0),
						stepsPerTap_(1),
						maxStepLengthInTimeUnits(0),
						maxRelativeDeviation(0),
						firstOfCycle(true),
						makeStep_(0) {

}
Tapper::~Tapper() {
	delete history;
}

void Tapper::init(uint16_t maxStepLengthInTimeUnits, uint8_t averageWidth, uint8_t maxRelativeDeviation)
{
	this->maxStepLengthInTimeUnits = maxStepLengthInTimeUnits;
	history = new MovingAverageLinear<uint16_t>(averageWidth);
	this->maxRelativeDeviation = maxRelativeDeviation;
	firstOfCycle = true;
	setStepsPerTap(1);
}



// should be done like this:
// reset cycle if tap is not near an expected position
// in this case clear history and set lastTapTime
// if call is in expected window add current tapTime difference to buffer

void Tapper::tap(uint16_t tapTime)
{
	#ifdef TESTING
	printf("NEW Tap @ time %u.\n",tapTime);
	#endif

	uint16_t thisTapDifference = tapTime - lastTapTime;


	if (history->getFillCount()>0) {

		uint8_t deviation = abs(thisTapDifference - history->operator[](0))/(history->getAverage()>>5);

		if (deviation<maxRelativeDeviation) {

			#ifdef TESTING
			printf("  Tap Difference %u was still in window (Deviation of %u)\n",thisTapDifference,deviation);
			#endif
			// tap is inside window
			// add difference to moving average
			history->add(thisTapDifference);

		} else {
			// we are inside a cycle but it is reset because tap is not in expected window
			// set current tempo as average to have a valid value in until next tap
			#ifdef TESTING
			printf("  Tap Difference %u out of window (Deviation of %u)\n   -> Restarting cycle\n",thisTapDifference,deviation);
			#endif
			history->clear();
			firstOfCycle = true;
		}

	} else {
		if ((!firstOfCycle) && (thisTapDifference < maxStepLengthInTimeUnits)) {
			// insert first tempo
			#ifdef TESTING
			printf("  Second tap -> adding first tempo to history: %u\n",thisTapDifference);
			#endif
			history->add(thisTapDifference);
		} else {

			#ifdef TESTING
			printf("  Starting very first tap cyle\n");
			#endif
			firstOfCycle = false;
		}
	}

	// always remember last tap time
	lastTapTime = tapTime;


	// call callback
    if (makeStep_ != 0) {
		#ifdef TESTING
    	printf("Tapper::tap Sending step command.\n");
		#endif
    	makeStep_();
    }





}
