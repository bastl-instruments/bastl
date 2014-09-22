/*
 * Tapper.cpp
 *
 *  Created on: Sep 5, 2014
 *      Author: bastl
 */

#include "Tapper.h"
#include <stdlib.h>

//#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif

#define MAX_TAP_STEP_LENGTH 200
#define MAX_TAP_STEP_DEVIATION 50

Tapper::Tapper() : 		stepsInRow_(-1),
						lastTapTime_(0),
						currentTapTime_(0),
						stepsPerTap_(1),
						maxStepLengthInTimeUnits_(200),
						maxStepDeviationInTImeUnits_(50),
						makeStep_(0) {

}

void Tapper::init(unsigned int maxStepLengthInTimeUnits, unsigned int  maxStepDeviationInTImeUnits)
{
	maxStepLengthInTimeUnits_ = maxStepLengthInTimeUnits;
	maxStepDeviationInTImeUnits_ = maxStepDeviationInTImeUnits;
}

void Tapper::tap(unsigned int tapTime)
{
	#ifdef DEBUG
	printf("Tapper::tap received tap with tap time %d.\n",tapTime);
	#endif
	// Reset values when first tap or tap over defined max tap deviation
    if ((stepsInRow_ == -1) ||
        (lastTapTime_ + MAX_TAP_STEP_LENGTH < tapTime) ||
        ((stepsInRow_ > 0) &&  (abs(lastTapTime_ + currentTapTime_ - tapTime)  > (0.5 * currentTapTime_))))
    {
    	lastTapTime_ = tapTime;
    	currentTapTime_ = 0;
        stepsInRow_ = 0;
        if (makeStep_ != 0) {
			#ifdef DEBUG
        	printf("Tapper::tap Sending step command (First Step).\n");
			#endif
        	makeStep_();
        }
    }
    else
    {
        unsigned int lastTapLength = tapTime - lastTapTime_;
        currentTapTime_ = ((currentTapTime_ * stepsInRow_) + lastTapLength) / (stepsInRow_ + 1);
        stepsInRow_++;
        lastTapTime_ = tapTime;
        if (makeStep_ != 0) {
			#ifdef DEBUG
        	printf("Tapper::tap Sending step command (Continous Step).\n");
			#endif
        	makeStep_();
        }
    }
}
