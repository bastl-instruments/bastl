/*
 * LFO.cpp
 *
 *  Created on: 07.04.2015
 *      Author: user
 */


#include "LFO.h"


LFO::LFO() :currentPhase(0), lastTimestamp(0), flopVector(0), xorVector(0), thres(255)
{
}


void LFO::setFrequency(uint16_t bastlCyclesPerPeriod) {

	if (bastlCyclesPerPeriod == 0) {
		phaseIncrement = 0;
		return;
	}

	phaseIncrement = ((uint32_t)65536 + (bastlCyclesPerPeriod>>1))/bastlCyclesPerPeriod;
}


void LFO::setToStep(uint8_t stepNumber, uint16_t timestamp) {
	lastTimestamp = timestamp;
	currentPhase = stepNumber << 8;
}

void LFO::setFlop(uint8_t bitVector) {
	flopVector = bitVector;
}
void LFO::setXOR(uint8_t bitVector) {
	xorVector = bitVector;
}
void LFO::setFolding(uint8_t thres) {
	this->thres = thres;
	isFolding = true;
}
void LFO::setOverflow(uint8_t thres) {
	this->thres = thres;
	isFolding = false;
}


void LFO::step() {
	currentPhase += phaseIncrement;
}

uint8_t LFO::getValue(uint16_t timestamp) {
	while(lastTimestamp != timestamp) {
		step();
		lastTimestamp++;
	}
	currentStep = currentPhase >> 8;
	return calcOutput();
}


