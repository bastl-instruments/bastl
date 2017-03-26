#include "NoVelocityStepMemory.h"
#include "BitArrayOperations.h"

#define INSTRUMENT_DATA_OFFSET

//#define DEBUG
#ifdef DEBUG

#include <cstdio>

#endif

NoVelocityStepMemory::NoVelocityStepMemory()
{
}

NoVelocityStepMemory::~NoVelocityStepMemory()
{
}

void NoVelocityStepMemory::getDrumStepDataPointers(unsigned char instrumentID, unsigned char step,
										   unsigned char *& mutes, unsigned char *& data) {

	unsigned int offset  = getDataOffset(instrumentID, step / 16);

	mutes = &data_[offset + (step / 8) % 2];
	data = &data_[offset + 2 + ((step % 16) / 2)];
}

DrumStep NoVelocityStepMemory::getDrumStep(unsigned char instrumentID, unsigned char step)
{
    unsigned char * data;
    unsigned char * mutes;

    unsigned char bitIndex = step % 8;
    getDrumStepDataPointers(instrumentID, step, mutes, data);

    DrumStep::DrumVelocityType subSteps[4];
    for (unsigned char i = 0; i < 4; i++) {
    	unsigned char offset = ((step % 2) * 4) + i;
    	#ifdef DEBUG
    	bool bitAtPosition = BitArrayOperations::getBit(* data, offset);
    	printf("Getting bit at index %d to value %s \n", offset, bitAtPosition ? "True": "False");
		#endif
        subSteps[i] = BitArrayOperations::getBit(* data, offset) ? DrumStep::NORMAL : DrumStep::OFF;
    }
    return DrumStep(BitArrayOperations::getBit(* mutes, bitIndex), subSteps);
}

unsigned char NoVelocityStepMemory::getNumberOfActives(unsigned char instrument) {
	unsigned char numberOfActives = 0;
	unsigned char instrumentOffset = (unsigned int)instrument * 48;
	for (unsigned char i = 0; i < 8; i++) {
		unsigned char offset = ((i / 2) * 12) + (i % 2);
		unsigned char data = data_[instrumentOffset + offset];
		for (numberOfActives; data; numberOfActives++) data &= data - 1;
	}
	return numberOfActives;

}

bool NoVelocityStepMemory::getNextActiveDrumStep(unsigned char instrumentID, unsigned char &step, DrumStep &drumStep)
{
	unsigned char activeSteps = getNumberOfActives(instrumentID);
	if (activeSteps != 0) {
		step = step % activeSteps;
		drumStep = getDrumStep(instrumentID, step);
		return true;

	} else {
		return false;
	}
}

bool NoVelocityStepMemory::setDrumStep(unsigned char instrumentID, unsigned char step, DrumStep stepData)
{
	unsigned char * mutes;
	unsigned char * data;

	unsigned char bitIndex = step % 8;

	getDrumStepDataPointers(instrumentID, step, mutes, data);

	BitArrayOperations::setBit(*mutes, bitIndex, stepData.isMuted());
	unsigned char offset = ((step % 2) * 4);
    for (int i = 0; i < 4 ; i++) {
    	BitArrayOperations::setBit(*data, offset + i, stepData.getSubStep(i) == DrumStep::NORMAL);
		#ifdef DEBUG
    	//printf("Setting bit at index %d to value %s \n", offset, stepData.getSubStep(i) == DrumStep::NORMAL ? "True": "False");
		#endif
    }
    return true;
}

void NoVelocityStepMemory::getMutesForNote(unsigned char instrumentID, unsigned char windowIndex, unsigned char *& data) {

	unsigned int offset  = getDataOffset(instrumentID, windowIndex);
	data = &data_[offset];
}

void NoVelocityStepMemory::getAllInstrumentActivesFor16Steps(unsigned char fromIndex, ActiveMultiStatus * result) {
	for (unsigned char step = 0; step < 16; step++) {
		unsigned char actives = 0;
		unsigned char inactives = 0;
		for (unsigned char instrument = 0; instrument < 6; instrument++) {
			if (step < getNumberOfActives(instrument)) {
				actives++;
			} else {
				inactives++;
			}
		}
		if (actives == 0) {
			result[step] = ALLINACTIVE;
		} else if (inactives == 0) {
			result[step] = ALLACTIVE;
		} else {
			result[step] = MIXED;
		}
	}
}

void NoVelocityStepMemory::makeActiveUpTo(unsigned char instrument, unsigned char indexUpTo) {
	indexUpTo++;
	unsigned char instrumentOffset = (unsigned int)instrument * 48;
	for (unsigned char i = 0; i < 8; i++) {
		unsigned char offset = instrumentOffset + (((i / 2) * 12) + (i % 2));
		data_[offset] = 0;
		for (unsigned char j = 0; j < 8; j++) {
			if (indexUpTo > 0) {
				data_[offset] |= (1 << j);
				indexUpTo--;
			}
		}
	}
}

void NoVelocityStepMemory::makeAllInstrumentsActiveUpTo(unsigned char indexUpTo) {
	for (unsigned char instrument = 0; instrument < 6; instrument++) {
		makeActiveUpTo(instrument, indexUpTo);
	}

}

void NoVelocityStepMemory::clearStepsForInstrument(unsigned char instrument) {
	for (unsigned char step = 0; step < 64; step++) {
		DrumStep stepData = getDrumStep(instrument, step);
		stepData.setMuted(true);
		for (unsigned char substep = 0; substep < 4; substep++) {
			stepData.setSubStep(substep, DrumStep::OFF);
		}
		setDrumStep(instrument, step, stepData);
	}
}

void NoVelocityStepMemory::clearStepsForAllInstruments() {
	for (unsigned char instrument = 0; instrument < 6; instrument++) {
		clearStepsForInstrument(instrument);
	}
}

unsigned int NoVelocityStepMemory::getDataOffset(unsigned char instrumentID, unsigned char pan) {
	// 64 steps,  bits per step = 384 bits = 48 bytes
	return ((unsigned int)instrumentID * 48) +
			((unsigned int)pan * 12) + 2;
}
