/*
 * BPMConverter.cpp
 *
 *  Created on: Sep 12, 2014
 *      Author: bastl
 */

#include "BPMConverter.h"

uint16_t BPMConverter::bpmToTimeUnits(uint16_t bpm, uint16_t timeUnitsPerSecond) {
	return ((timeUnitsPerSecond * 60) / bpm) / 16;
}

uint16_t BPMConverter::timeUnitsToBPM(uint16_t timeUnitsPerBeat, uint16_t timeUnitsPerSecond) {
	return (timeUnitsPerSecond * 60) / timeUnitsPerBeat / 16;
}
