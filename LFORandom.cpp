/*
 * LFORandom.cpp
 *
 *  Created on: 07.04.2015
 *      Author: user
 */


#include "LFORandom.h"
#include "random.h"

void LFORandom::step() {
	LFO::step();

	// pick a new value whenever phase wraps, which indicates the start of a period
	if (currentPhase < phaseIncrement) thisStepDetailed = ((bastlRandom::byte()<<8) | bastlRandom::byte());

	addToBuffer(((uint32_t)getBufferAverage()*(smoothness)+(uint32_t)thisStepDetailed*(255-smoothness))/255);
}





void LFORandom::setSmoothness(uint8_t val) {
	smoothness = val;
}




// mapping from smoothness to reduction of buffer length
uint8_t mappedBufferLen(uint8_t val) {
	if (val < 5) return 31;
	else if (val < 7) return 25;
	else if (val < 10) return 15;
	else if (val < 20) return 10;
	else if (val < 30) return 5;
	else return 0;
}


void LFORandom::addToBuffer(uint16_t val) {
	buffer[bufferPos] = val;
	bufferPos++;
	if (bufferPos == bufferSize) bufferPos = 0;
}


uint16_t LFORandom::getBufferAverage() {

	// get average of a certain number of previous elements depending on smoothness
	uint8_t numbElements = bufferSize-mappedBufferLen(smoothness);

	// start from last stored value
	uint8_t index = bufferPos-1;
	if (index == 255) index = bufferSize-1;

	// sum all those elements...
	uint32_t sum = 0;
	for (uint8_t count = 0; count<numbElements; count++,index--) {
		if (index == 255) index = bufferSize-1;
		sum += buffer[index];
	}

	// ...and return the average
	return sum/numbElements;
}




uint8_t LFORandom::calcOutput() {

	// Apply flopping
	if (currentStep & flopVector) {
		return 0;
	}

	currentOutput = getBufferAverage() >> 8;

	// Apply Overflowing
	if (currentOutput > thres) {
		if (isFolding) {
			uint8_t sectorNumber = currentOutput/thres;
			if (sectorNumber & 1) currentOutput = thres - (currentOutput % thres);
			else currentOutput = currentOutput % thres;
		} else {
			currentOutput = currentOutput % thres;
		}
	}

	// apply xor
	currentOutput ^= xorVector;

	return currentOutput;

}
