#include "FlashStepMemory.h"
#include "InstrumentDefinitions.h"

#define INSTRUMENT_DATA_OFFSET

//#define DEBUG
#ifdef DEBUG

#include <iostream>

#endif

FlashStepMemory::FlashStepMemory(IHWLayer *hwLayer) : hwLayer_(hwLayer)
{
}

DrumStep FlashStepMemory::getDrumStep(unsigned char instrumentID, unsigned char pattern, unsigned char step)
{
    unsigned char byteIndex = step / 8;
    unsigned char bitIndex = step % 8;

    long instrumentOffset = (STEPS_PER_PATTERN * INSTRUMENTS * pattern) /* patterbs offset */ +
                            (STEPS_PER_PATTERN * instrumentID) /* instruments offset */ ;

    unsigned char actives = hwLayer_->readSRAM(instrumentOffset + byteIndex);
    unsigned char mutes = hwLayer_->readSRAM(instrumentOffset + byteIndex + 8);
    unsigned char data = hwLayer_->readSRAM(instrumentOffset + 16 + step);

    bool mute = ((mutes & (1 << bitIndex)) >> bitIndex) == 1;
    bool active = ((actives & (1 << bitIndex)) >> bitIndex) == 1;

    DrumStep::DrumVelocityType subSteps[4] = {DrumStep::OFF, DrumStep::OFF, DrumStep::OFF, DrumStep::OFF};
    for (unsigned char i = 0; i < 4; i++) {
        subSteps[i] = (DrumStep::DrumVelocityType)(((3 << (i * 2)) & data) >> (i * 2));
    }
    return DrumStep(active, mute, subSteps);
}



bool FlashStepMemory::getNextActiveDrumStep(unsigned char instrumentID, unsigned char pattern, unsigned char &step, DrumStep &drumStep)
{
    unsigned char byteIndex = step / 8;
    unsigned char originalByteIndex = byteIndex;
    unsigned char bitIndex = step % 8;

    long instrumentOffset = (STEPS_PER_PATTERN * INSTRUMENTS * pattern) /* patterbs offset */ +
                            (STEPS_PER_PATTERN * instrumentID) /* instruments offset */ ;

    // Searching for first active

    unsigned char actives = hwLayer_->readSRAM(instrumentOffset + byteIndex);

    unsigned char nextActives = actives >> bitIndex;


    while (nextActives == 0) {
    	bitIndex = 0;
        byteIndex = ((byteIndex + 1) % 8);
        nextActives = hwLayer_->readSRAM(instrumentOffset + byteIndex);
        if (byteIndex == originalByteIndex && nextActives == 0) {
            return false;
        }
    }

    actives = nextActives;

    //Find first non zero bit in the array
    while (actives % 2 == 0) {
        actives = actives >> 1;
        bitIndex++;
    }

    step = byteIndex * 8 + bitIndex;

    unsigned char mutes = hwLayer_->readSRAM(instrumentOffset + byteIndex + 8);
    unsigned char data = hwLayer_->readSRAM(instrumentOffset + 16 + step);

    bool mute = ((mutes & (1 << bitIndex)) >> bitIndex) == 1;
    bool active = ((actives & (1 << bitIndex)) >> bitIndex) == 1;

    DrumStep::DrumVelocityType subSteps[4] = {DrumStep::OFF, DrumStep::OFF, DrumStep::OFF, DrumStep::OFF};
    for (unsigned char i = 0; i < 4; i++) {
        subSteps[i] = (DrumStep::DrumVelocityType)(((3 << (i * 2)) & data) >> (i * 2));
    }

    drumStep = DrumStep(active, mute, subSteps);
    return true;
}

bool FlashStepMemory::setDrumStep(unsigned char instrumentID, unsigned char pattern, unsigned char step, DrumStep stepData)
{
    unsigned char byteIndex = step / 8;
    unsigned char bitIndex = step % 8;

    long instrumentOffset = (STEPS_PER_PATTERN * INSTRUMENTS * pattern) /* patterbs offset */ +
                            (STEPS_PER_PATTERN * instrumentID) /* instruments offset */ ;

    unsigned char actives = hwLayer_->readSRAM(instrumentOffset + byteIndex);
    unsigned char mutes = hwLayer_->readSRAM(instrumentOffset + byteIndex + 8);
    unsigned char data = 0;

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
        data = (data | stepData.getSubStep(i) << (2 * i));
    }
    hwLayer_->writeSRAM(instrumentOffset + byteIndex, actives);
    hwLayer_->writeSRAM(instrumentOffset + byteIndex + 8, mutes);
    hwLayer_->writeSRAM(instrumentOffset + 16 + step, data);
    return true;
}

void FlashStepMemory::getActivesAndMutesForNote(unsigned char instrumentID, unsigned char pattern, unsigned char windowIndex, unsigned char * data) {
	long instrumentOffset = (STEPS_PER_PATTERN * INSTRUMENTS * pattern) /* patterbs offset */ +
	                            (STEPS_PER_PATTERN * instrumentID) /* instruments offset */ ;

	data[0] = hwLayer_->readSRAM(instrumentOffset + windowIndex);
	data[1] = hwLayer_->readSRAM(instrumentOffset + windowIndex + 1);
	data[2] = hwLayer_->readSRAM(instrumentOffset + windowIndex + 8);
	data[3] = hwLayer_->readSRAM(instrumentOffset + windowIndex + 9);
}

void FlashStepMemory::getPatternSettings(unsigned char patternIndex, unsigned char * settings) {
	hwLayer_->readSRAM( DRUM_BYTES + patternIndex * 3, settings, 3);
}
void FlashStepMemory::setPatternSettings(unsigned char patternIndex, unsigned char * settings) {
	hwLayer_->writeSRAM( DRUM_BYTES + patternIndex * 3, settings, 3);
}

