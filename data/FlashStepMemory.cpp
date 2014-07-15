#include "FlashStepMemory.h"
#include "InstrumentDefinitions.h"

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

    actives = actives >> bitIndex;

    while (actives == 0) {
        byteIndex = ((byteIndex + 1) % 4);
        actives = hwLayer_->readSRAM(instrumentOffset + byteIndex);
        if (byteIndex == originalByteIndex) {
            return false;
        }
    }

    //Find first non zero bit in the array
    while (actives % 2 == 0) {
        actives = actives >> 1;
        bitIndex++;
    }

    unsigned char mutes = hwLayer_->readSRAM(instrumentOffset + byteIndex + 8);
    unsigned char data = hwLayer_->readSRAM(instrumentOffset + 16 + step);

    bool mute = ((mutes & (1 << bitIndex)) >> bitIndex) == 1;
    bool active = ((actives & (1 << bitIndex)) >> bitIndex) == 1;

    DrumStep::DrumVelocityType subSteps[4] = {DrumStep::OFF, DrumStep::OFF, DrumStep::OFF, DrumStep::OFF};
    for (unsigned char i = 0; i < 4; i++) {
        subSteps[i] = (DrumStep::DrumVelocityType)(((3 << (i * 2)) & data) >> (i * 2));
    }

    step = byteIndex * 8 + bitIndex;

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

    for (int i = 3; i >=0 ; i--) {
        data = (data | stepData.getSubStep(i));
        if (i != 0) {
            data = data << 2;
        }
    }
    hwLayer_->writeSRAM(instrumentOffset + byteIndex, actives);
    hwLayer_->writeSRAM(instrumentOffset + byteIndex + 8, mutes);
    hwLayer_->writeSRAM(instrumentOffset + 16 + step, data);
    return true;
}
