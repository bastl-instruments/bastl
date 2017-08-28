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
										   unsigned char *& mutes, unsigned char *& actives,
										   unsigned char *& data) {

	unsigned int offset  = getDataOffset(instrumentID, step / 16);

	actives = &data_[offset + (step / 8) % 2 ];
	mutes = &data_[offset + 2 + (step / 8) % 2];
	data = &data_[offset + 4 + ((step % 16) / 2)];
}

DrumStep NoVelocityStepMemory::getDrumStep(unsigned char instrumentID, unsigned char step)
{
    unsigned char * data;
    unsigned char * actives;
    unsigned char * mutes;

    unsigned char bitIndex = step % 8;
    getDrumStepDataPointers(instrumentID, step, mutes, actives, data);

    DrumStep::DrumVelocityType subSteps[4];
    for (unsigned char i = 0; i < 4; i++) {
    	unsigned char offset = ((step % 2) * 4) + i;
    	#ifdef DEBUG
    	bool bitAtPosition = BitArrayOperations::getBit(* data, offset);
    	printf("Getting bit at index %d to value %s \n", offset, bitAtPosition ? "True": "False");
		#endif
        subSteps[i] = BitArrayOperations::getBit(* data, offset) ? DrumStep::NORMAL : DrumStep::OFF;
    }
    return DrumStep(BitArrayOperations::getBit(* actives, bitIndex), BitArrayOperations::getBit(* mutes, bitIndex), subSteps);
}



bool NoVelocityStepMemory::getNextActiveDrumStep(unsigned char instrumentID, unsigned char &step, DrumStep &drumStep)
{
	drumStep = getDrumStep(instrumentID, step);
	if (drumStep.isActive()) {
		return true;
	}
	unsigned char pan = step / 16;
	unsigned int offset  = getDataOffset(instrumentID, pan);
	unsigned int panActiveData = (((unsigned int)data_[offset + 1]) << 8) + data_[offset];
	for (unsigned char stepIndex = 0; stepIndex < 64; stepIndex++) {
		unsigned char realStepIndex = (stepIndex + step) % 64;
		if (realStepIndex / 16 != pan) {
			pan = realStepIndex / 16;
			offset  = getDataOffset(instrumentID, pan);
			panActiveData = (((unsigned int)data_[offset + 1]) << 8) + (unsigned int)data_[offset];
		}
		if ((panActiveData & ((unsigned int)1 << (realStepIndex % 16))) != 0) {
			drumStep = getDrumStep(instrumentID, realStepIndex);
			step = realStepIndex;
			return true;
		}
	}
	return false;
}

bool NoVelocityStepMemory::setDrumStep(unsigned char instrumentID, unsigned char step, DrumStep stepData)
{
	unsigned char * mutes;
	unsigned char * actives;
	unsigned char * data;

	unsigned char bitIndex = step % 8;

	getDrumStepDataPointers(instrumentID, step, mutes, actives, data);

	BitArrayOperations::setBit(*actives, bitIndex, stepData.isActive());
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

void NoVelocityStepMemory::getActivesAndMutesForNote(unsigned char instrumentID, unsigned char windowIndex, unsigned char *& data) {

	unsigned int offset  = getDataOffset(instrumentID, windowIndex);
	data = &data_[offset];
}

void NoVelocityStepMemory::getAllInstrumentActivesFor16Steps(unsigned char fromIndex, ActiveMultiStatus * result) {
	for (unsigned char step = 0; step < 16; step++) {
		unsigned char actives = 0;
		unsigned char inactives = 0;
		for (unsigned char instrument = 0; instrument < 6; instrument++) {
			bool active = getDrumStep(instrument, fromIndex + step).isActive();
			if (active) {
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

void NoVelocityStepMemory::getActiveWindowBitArray(unsigned char instrument, bool * result) {
	for (unsigned char pan = 0; pan < 4; pan++) {
		unsigned int offset  = getDataOffset(instrument, pan);
		result[pan] = (data_[offset] != 0) || (data_[offset + 1] != 0);
	}
}

void NoVelocityStepMemory::getAllInstrumentsActiveWindowBitArray(bool * result) {
	result[0] = result[1] = result[2] = result[3] = false;
	for (unsigned char instrument = 0; instrument < 6; instrument++) {
		for (unsigned char pan = 0; pan < 4; pan++) {
			unsigned int offset  = getDataOffset(instrument, pan);
			result[pan] = result[pan] || (data_[offset] != 0) || (data_[offset + 1] != 0);
		}
	}
}

void NoVelocityStepMemory::makeActiveUpTo(unsigned char instrument, unsigned char indexUpTo) {
	unsigned char stepIndex = 0;
	while (stepIndex + 7 <= indexUpTo) {
		data_[getDataOffset(instrument, stepIndex / 16) + ((stepIndex / 8) % 2)] = 255;
		stepIndex += 8;
	}
	if ((indexUpTo + 1) % 8 != 0) {
		for (unsigned char step; step < 8; step++) {
			BitArrayOperations::setBit(
					data_[getDataOffset(instrument, stepIndex / 16) + ((stepIndex / 8) % 2)],
					stepIndex % 8,
					stepIndex <= indexUpTo);
			stepIndex++;
		}
	}
	while (stepIndex < 64) {
		data_[getDataOffset(instrument, stepIndex / 16) + ((stepIndex / 8) % 2)] = 0;
		stepIndex += 8;
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

unsigned char NoVelocityStepMemory::getNumberOfActives(unsigned char instrument) {
	unsigned char numberOfActives = 0;
	unsigned int instrumentOffset = (unsigned int)instrument * (unsigned int)42;
	for (unsigned char i = 0; i < 8; i++) {
		unsigned int offset = ((i / 2) * 12) + (i % 2);
		unsigned char data = data_[instrumentOffset + offset];
		for (unsigned char dataIndex = 0; dataIndex < 8; dataIndex++) {
			numberOfActives += (data & (1 << dataIndex)) != 0 ? 1 : 0;
		}
	}
	return numberOfActives;
}

unsigned int NoVelocityStepMemory::getDataOffset(unsigned char instrumentID, unsigned char pan) {
	// 64 steps, 6 bits per step = 384 bits = 48 bytes
	return (unsigned int)instrumentID * 48 + (unsigned int)pan * 12;
}
