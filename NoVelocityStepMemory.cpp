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
	unsigned char byteIndex = step / 8;
	unsigned int instrumentOffset  = getInstrumentDataOffset(instrumentID);

	actives = &data_[instrumentOffset + byteIndex];
	mutes = &data_[instrumentOffset + byteIndex + 8];
	data = &data_[instrumentOffset + 16 + (step / 2)];
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
	unsigned char originalStep = step;
	drumStep = getDrumStep(instrumentID, step);
	while (!drumStep.isActive()) {
		step = (step + 1) % 64;
		if (step == originalStep) {
			return false;
		}
		drumStep = getDrumStep(instrumentID, step);
	}
	return true;
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

void NoVelocityStepMemory::getActivesAndMutesForNote(unsigned char instrumentID, unsigned char windowIndex, unsigned char * data) {

	unsigned int instrumentOffset  = getInstrumentDataOffset(instrumentID);

	data[0] = data_[instrumentOffset + windowIndex];
	data[1] = data_[instrumentOffset + windowIndex + 1];
	data[2] = data_[instrumentOffset + windowIndex + 8];
	data[3] = data_[instrumentOffset + windowIndex + 9];
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
		unsigned int instrumentOffset  = getInstrumentDataOffset(instrument);
		result[pan] = (data_[instrumentOffset + pan * 2] != 0) || (data_[instrumentOffset + pan * 2 + 1] != 0);
	}
}

void NoVelocityStepMemory::getAllInstrumentsActiveWindowBitArray(bool * result) {
	result[0] = result[1] = result[2] = result[3] = false;
	for (unsigned char instrument = 0; instrument < 6; instrument++) {
		for (unsigned char pan = 0; pan < 4; pan++) {
			unsigned int instrumentOffset  = getInstrumentDataOffset(instrument);
			result[pan] = result[pan] || (data_[instrumentOffset + pan * 2] != 0) || (data_[instrumentOffset + pan * 2 + 1] != 0);
		}
	}
}

void NoVelocityStepMemory::makeActiveUpTo(unsigned char instrument, unsigned char indexUpTo) {
	for (int stepIndex = 0; stepIndex < 64; stepIndex++) {
		DrumStep step = getDrumStep(instrument, stepIndex);
		step.setActive(stepIndex <= indexUpTo);
		setDrumStep(instrument, stepIndex, step);
	}
}

void NoVelocityStepMemory::makeAllInstrumentsActiveUpTo(unsigned char indexUpTo) {
	for (unsigned char instrument = 0; instrument < 6; instrument++) {
		makeActiveUpTo(instrument, indexUpTo);
	}

}

bool NoVelocityStepMemory::isInDefaultState(unsigned char instrumentID) {

	unsigned int instrumentOffset  = getInstrumentDataOffset(instrumentID);
	for (unsigned char index = 2; index < 8; index++) {
		if (data_[instrumentOffset + 8 + index] != 255) {
			return false;
		}
	}
	//go through all the steps (1 step = 4 bits)
	for (unsigned char step = 8; step < 32; step++) {
		if (data_[instrumentOffset + 16 + step] != 0) {
			return false;
		}
	}
	return true;
}

void NoVelocityStepMemory::copyPan(unsigned char instrumentID, unsigned char panFrom, unsigned char panTo) {

	unsigned int instrumentOffset  = getInstrumentDataOffset(instrumentID);

	data_[instrumentOffset + 8 + panTo * 2] = data_[instrumentOffset + 8 + panFrom * 2];
	data_[instrumentOffset + 8 + panTo * 2 + 1] = data_[instrumentOffset + 8 + panFrom * 2 + 1];

	// copy 16 steps (4 bits per step = 8 bytes)
	for (unsigned char step = 0; step < 8; step++) {
		data_[instrumentOffset + 16 + panTo * 8 + step] = data_[instrumentOffset + 16 + panFrom * 8 + step];
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
