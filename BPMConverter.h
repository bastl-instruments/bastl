/*
 * BPMConverter.h
 *
 *  Created on: Sep 12, 2014
 *      Author: bastl
 */

#ifndef BPMCONVERTER_H_
#define BPMCONVERTER_H_

#include <inttypes.h>

class BPMConverter {
public:
	static uint8_t bpmToTimeUnits(uint16_t bpm, uint16_t timeUnitsPerSecond);
	static uint8_t timeUnitsToBPM(uint16_t timeUnits, uint16_t timeUnitsPerSecond);
};

#endif /* BPMCONVERTER_H_ */
