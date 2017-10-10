/*
 * BPMConverter.cpp
 *
 *  Created on: Sep 12, 2014
 *      Author: bastl
 */

#include "BPMConverter.h"

uint16_t BPMConverter::bpmToTimeUnits(uint16_t bpm, uint16_t timeUnitsPerSecond, uint16_t sourceLeftOvers, uint16_t & leftOvers) {
	uint16_t base = (timeUnitsPerSecond * 60) + ((sourceLeftOvers * 60) / 1000);
	uint16_t division = (bpm * 16);
	uint32_t returnValue = base / division;
	uint32_t rest = (((uint32_t)base % (uint32_t)division) * 1000UL);
	leftOvers = (uint16_t)(rest / (uint32_t)division);
	return returnValue;
}

uint16_t BPMConverter::timeUnitsToBPM(uint16_t timeUnitsPerBeat, uint16_t timeUnitsPerSecond) {
	return (timeUnitsPerSecond * 60) / timeUnitsPerBeat / 16;
}
