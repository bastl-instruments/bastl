/*
 * BPMConverter.cpp
 *
 *  Created on: Sep 12, 2014
 *      Author: bastl
 */

#include <BPMConverter.h>

unsigned char BPMConverter::bpmToTimeUnits(unsigned int bpm, unsigned int timeUnitsPerSecond) {
	return (timeUnitsPerSecond * 60) / bpm;
}

unsigned char BPMConverter::timeUnitsToBPM(unsigned int timeUnitsPerBeat, unsigned int timeUnitsPerSecond) {
	return (timeUnitsPerSecond * 60) / timeUnitsPerBeat;
}
