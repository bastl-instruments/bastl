/*
 * Tapper.cpp
 *
 *  Created on: Sep 5, 2014
 *      Author: bastl
 */

#include "Tapper.h"

//#define DEBUG
#ifdef DEBUG
#include <stdio.h>
#endif


#include <stdlib.h>


Tapper::Tapper() :		history(0),
						lastTapTime(0),
						stepsPerTap_(1),
						maxStepLengthInTimeUnits(0),
						maxRelativeDeviation(0),
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
}



// should be done like this:
// reset cycle if tap is not near an expected position
// in this case clear history and set lastTapTime
// if call is in expected window add current tapTime difference to buffer

void Tapper::tap(uint16_t tapTime)
{
	#ifdef DEBUG
	printf("Tapper::tap received tap with tap time %u.\n",tapTime);
	#endif

	static bool firstOfCycle = true;

	uint8_t thisTapDifference = tapTime - lastTapTime;

	if (history->getFillCount()>0) {

		uint8_t deviation = abs(thisTapDifference - history->operator[](0))/(history->getAverage()>>5);

		if (deviation<maxRelativeDeviation) {

			#ifdef DEBUG
			printf("Speed %u was still in window (Deviation of %u)\n",thisTapDifference,deviation);
			#endif
			// tap is inside window
			// calculate current difference and add it to history
			history->add(thisTapDifference);

		} else {
			// we are inside a cycle but it is reset because tap is not in expected window
			#ifdef DEBUG
			printf("Out of window (%u) -> Restarting cycle\n",thisTapDifference);
			#endif
			history->clear(history->getAverage());
		}

	} else {
		if (!firstOfCycle) {
			// insert first tempo
			#ifdef DEBUG
			printf("Second tap -> adding first tempo to history: %u\n",thisTapDifference);
			#endif
			history->add(thisTapDifference);
		} else {

			#ifdef DEBUG
			printf("Starting very first tap cyle\n");
			#endif

			firstOfCycle = false;
		}
	}

	// always remember last tap time
	lastTapTime = tapTime;


	// call callback
    if (makeStep_ != 0) {
		#ifdef DEBUG
    	printf("Tapper::tap Sending step command.\n");
		#endif
    	makeStep_();
    }





}
