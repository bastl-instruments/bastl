/*
 * BPMConverter.h
 *
 *  Created on: Sep 12, 2014
 *      Author: bastl
 */

#ifndef BPMCONVERTER_H_
#define BPMCONVERTER_H_

class BPMConverter {
	static unsigned char bpmToTimeUnits(unsigned int bpm, unsigned int timeUnitsPerSecond);
	static unsigned char timeUnitsToBPM(unsigned int timeUnits, unsigned int timeUnitsPerSecond);
};

#endif /* BPMCONVERTER_H_ */
