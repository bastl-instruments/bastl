/*
 * SimplifiedTapper.cpp
 *
 *  Created on: Sep 5, 2014
 *      Author: bastl
 */

#include "SimplifiedTapper.h"
#include <stdlib.h>

//#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif

SimplifiedTapper::SimplifiedTapper() : stepsInRow_(-1),
									   lastTapTime_(0),
									   currentTapTime_(0),
									   stepsPerTap_(1),
									   maxStepLengthInTimeUnits_(200),
									   maxStepDeviationInTImeUnits_(50),
									   makeStep_(0) {

}

SimplifiedTapper::~SimplifiedTapper() {

}
void SimplifiedTapper::init(unsigned int maxStepLengthInTimeUnits, unsigned int  maxStepDeviationInTImeUnits)
{
	maxStepLengthInTimeUnits_ = maxStepLengthInTimeUnits;
	maxStepDeviationInTImeUnits_ = maxStepDeviationInTImeUnits;
}

void SimplifiedTapper::tap(unsigned int tapTime)
{
	#ifdef DEBUG
	printf("Tapper::tap received tap with tap time %d.\n",tapTime);
	#endif
	// Reset values when first tap or tap over defined max tap deviation
    if ((stepsInRow_ == -1) ||
        (lastTapTime_ + maxStepLengthInTimeUnits_ < tapTime) ||
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
        lastTapTime = lastTapLength;
        currentTapTime_ = (unsigned int)((((unsigned long)currentTapTime_ * (unsigned long)stepsInRow_) + (unsigned long)lastTapLength) / (unsigned long)(stepsInRow_ + 1));
        if (stepsInRow_ < 4) {
        	stepsInRow_++;
        }
        lastTapTime_ = tapTime;
        if (makeStep_ != 0) {
			#ifdef DEBUG
        	printf("Tapper::tap Sending step command (Continous Step).\n");
			#endif
        	makeStep_();
        }
    }
}
