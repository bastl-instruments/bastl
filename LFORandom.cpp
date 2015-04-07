/*
 * LFORandom.cpp
 *
 *  Created on: 07.04.2015
 *      Author: user
 */


#include "LFORandom.h"
#include "random.h"

void setSmoothness(uint8_t val) {

}

void LFORandom::step() {
	LFO::step();

	if (currentPhase < phaseIncrement) randomStep();
}


void LFORandom::randomStep() {
	thisStepDetailed = nextStepDetailed;
	nextStepDetailed = ((bastlRandom::byte()<<8) | bastlRandom::byte());
}


void LFORandom::addToBuffer(uint16_t val) {
	buffer[bufferPos] = val;
	bufferPos++;
	if (bufferPos == bufferSize) bufferPos = 0;
}


void LFORandom::setSmoothness(uint8_t val) {
	smoothness = val;
}

uint8_t mappedBufferLen(uint8_t val) {
	if (val < 5) return 31;
	else if (val < 6) return 25;
	else if (val < 10) return 10;
	else if (val < 20) return 5;
	else return 0;
}

uint16_t LFORandom::getBufferAverage() {
	uint32_t sum = 0;
	const uint8_t numbElements = bufferSize-mappedBufferLen(smoothness);
	uint8_t index = bufferPos-1;
	if (index == 255) index = bufferSize-1;
	for (uint8_t count = 0; count<numbElements; count++,index--) {
		if (index == 255) index = bufferSize-1;
		sum += buffer[index];
	}

	return sum/numbElements;
}




uint8_t LFORandom::calcOutput() {
	if (currentStep & flopVector) {
		return 0;
	}

	uint8_t shiftedStep = currentStep;
	if (shiftedStep > smoothness) shiftedStep = 255;

	addToBuffer (((uint32_t) nextStepDetailed*shiftedStep + (uint32_t)thisStepDetailed*(255-shiftedStep))>>8);
	//addToBuffer(thisStepDetailed);


	//return ((getBufferAverage()>>9) + (thisStepDetailed >>9));

	//return thisStepDetailed>>8;
	return getBufferAverage() >> 8;


}
