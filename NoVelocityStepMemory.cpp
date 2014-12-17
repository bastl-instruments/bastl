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

DrumStep NoVelocityStepMemory::getDrumStep(unsigned char instrumentID, unsigned char step)
{
    unsigned char byteIndex = step / 8;
    unsigned char bitIndex = step % 8;

    long instrumentOffset = (64 * instrumentID) /* instruments offset */ ;

    instrumentOffset  = (instrumentOffset * 6) / 8;

    unsigned char actives = data_[instrumentOffset + byteIndex];
    unsigned char mutes = data_[instrumentOffset + byteIndex + 8];
    unsigned char data = data_[instrumentOffset + 16 + (step / 2)];

	#ifdef DEBUG
    printf("Getting data at index %d to value %d \n", instrumentOffset + 16 + (step / 2), data);
	#endif

    bool mute = ((mutes & (1 << bitIndex)) >> bitIndex) == 1;
    bool active = ((actives & (1 << bitIndex)) >> bitIndex) == 1;

    DrumStep::DrumVelocityType subSteps[4] = {DrumStep::OFF, DrumStep::OFF, DrumStep::OFF, DrumStep::OFF};
    for (unsigned char i = 0; i < 4; i++) {
    	unsigned char offset = ((step % 2) * 4) + i;
    	#ifdef DEBUG
    	bool bitAtPosition = GETBIT(data, offset);
    	printf("Getting bit at index %d to value %s \n", offset, bitAtPosition ? "True": "False");
		#endif
        subSteps[i] = GETBIT(data, offset) ? DrumStep::NORMAL : DrumStep::OFF;
    }
    return DrumStep(active, mute, subSteps);
}



bool NoVelocityStepMemory::getNextActiveDrumStep(unsigned char instrumentID, unsigned char &step, DrumStep &drumStep)
{
    unsigned char byteIndex = step / 8;
    unsigned char originalByteIndex = byteIndex;
    unsigned char bitIndex = step % 8;

    long instrumentOffset = (64 * instrumentID) /* instruments offset */ ;

    instrumentOffset  = (instrumentOffset * 6) / 8;

    // Searching for first active

    unsigned char actives = data_[instrumentOffset + byteIndex];

    unsigned char nextActives = actives >> bitIndex;


    while (nextActives == 0) {
		#ifdef DEBUG
    	printf("Next active byte 0 trying to find next one \n");
		#endif
    	bitIndex = 0;
        byteIndex = ((byteIndex + 1) % 8);
        nextActives = data_[instrumentOffset + byteIndex];
        if (byteIndex == originalByteIndex && nextActives == 0) {
            return false;
        }
    }

    actives = nextActives;

    //Find first non zero bit in the array
    while (actives % 2 == 0) {
		#ifdef DEBUG
    	printf("Next step (%d) inactive trying to find next one \n", bitIndex);
		#endif
        actives = actives >> 1;
        bitIndex++;
    }

    step = byteIndex * 8 + bitIndex;

    drumStep = getDrumStep(instrumentID, step);
    return true;
}

bool NoVelocityStepMemory::setDrumStep(unsigned char instrumentID, unsigned char step, DrumStep stepData)
{

    unsigned char byteIndex = step / 8;
    unsigned char bitIndex = step % 8;

    long instrumentOffset = (64 * instrumentID);

    #ifdef DEBUG
	printf("Instrument offset is: %d \n", (int)instrumentOffset);
    #endif

    instrumentOffset  = (instrumentOffset * 6) / 8;

    #ifdef DEBUG
	printf("Instrument offset is: %d \n", (int)instrumentOffset);
    #endif

    unsigned char actives = data_[instrumentOffset + byteIndex];
    unsigned char mutes = data_[instrumentOffset + byteIndex + 8];
    unsigned char data = data_[instrumentOffset + 16 + (step / 2)];

    if (stepData.isActive()) {
        actives = actives | 1 << bitIndex;
    } else {
        actives = actives & ~(1 << bitIndex);
    }

    if (stepData.isMuted()) {
        mutes = mutes | 1 << bitIndex;
    } else {
        mutes = mutes & ~(1 << bitIndex);
    }

    for (int i = 0; i < 4 ; i++) {
    	unsigned char offset = ((step % 2) * 4) + i;
    	SETBIT(data, offset, stepData.getSubStep(i) == DrumStep::NORMAL);
		#ifdef DEBUG
    	printf("Setting bit at index %d to value %s \n", offset, stepData.getSubStep(i) == DrumStep::NORMAL ? "True": "False");
		#endif
    }
    data_[instrumentOffset + byteIndex] = actives;
    data_[instrumentOffset + byteIndex + 8] = mutes;
	#ifdef DEBUG
	printf("Setting data at index %d to value %d \n", instrumentOffset + 16 + (step / 2), data);
    #endif
    data_[instrumentOffset + 16 + (step / 2)] = data;
    return true;
}

void NoVelocityStepMemory::getActivesAndMutesForNote(unsigned char instrumentID, unsigned char windowIndex, unsigned char * data) {
	long instrumentOffset = (64 * instrumentID) /* instruments offset */ ;

	instrumentOffset  = (instrumentOffset * 6) / 8;

	data[0] = data_[instrumentOffset + windowIndex];
	data[1] = data_[instrumentOffset + windowIndex + 1];
	data[2] = data_[instrumentOffset + windowIndex + 8];
	data[3] = data_[instrumentOffset + windowIndex + 9];
}

void NoVelocityStepMemory::getAllInstrumentActivesFor16Steps(unsigned char windowIndex, ActiveMultiStatus * result) {
	bool firstInstrument = true;
	for (unsigned char instrument = 0; instrument < 6; instrument++) {
		long instrumentOffset = (64 * instrument) /* instruments offset */ ;
		instrumentOffset  = (instrumentOffset * 6) / 8;
		for (unsigned char step = 0; step < 16; step++) {
			unsigned char data = step < 8 ? data_[instrumentOffset + windowIndex] :
											data_[instrumentOffset + windowIndex + 1];
			bool active = GETBIT(data, step % 8);
			if (firstInstrument) {
				result[step] = active ? ALLACTIVE : ALLINACTIVE;
			} else {
				if (result[step] == ALLACTIVE && !active) {
					result[step] = MIXED;
				} else if (result[step] == ALLINACTIVE && active) {
					result[step] = MIXED;
				}
			}
		}
		firstInstrument = false;
	}
}

