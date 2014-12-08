#include "DummyStepMemory.h"

//#define DEBUG
#ifdef DEBUG

#include <iostream>

#endif

DummyStepMemory::DummyStepMemory() {
    DrumStep::DrumVelocityType substeps[4] = {DrumStep::OFF, DrumStep::OFF, DrumStep::OFF, DrumStep::OFF};
    for (int i = 0; i < DRUM_STEPS; i ++) {
        drumSteps_[i] = DrumStep(true, true, substeps);
    }
}

DrumStep DummyStepMemory::getDrumStep(unsigned char instrumentID, unsigned char pattern, unsigned char step) {
    return drumSteps_[instrumentID * DRUM_PATTERNS * STEPS_PER_PATTERN + pattern * STEPS_PER_PATTERN + step] ;
}

bool DummyStepMemory::setDrumStep(unsigned char instrumentID, unsigned char pattern, unsigned char step, DrumStep stepData) {
    drumSteps_[instrumentID * DRUM_PATTERNS * STEPS_PER_PATTERN + pattern * STEPS_PER_PATTERN + step] = stepData;
    return true;

}

bool DummyStepMemory::getNextActiveDrumStep(unsigned char instrumentID, unsigned char pattern, unsigned char & step, DrumStep & drumStep)
{
    unsigned char currentStepIndex = step;
    #ifdef DEBUG
    unsigned char index = (instrumentID * DRUM_PATTERNS * STEPS_PER_PATTERN) + (pattern * STEPS_PER_PATTERN) + step;
    printf("DummyStepMemory::getNextActiveDrumStep index in array : %i , length of array %i\ns", index, DRUM_STEPS);
    #endif
    drumStep = drumSteps_[instrumentID * DRUM_PATTERNS * STEPS_PER_PATTERN + pattern * STEPS_PER_PATTERN + step];
    while (!drumStep.isActive()) {
        step = (step + 1) % STEPS_PER_PATTERN;
        if (step == currentStepIndex) {
          return false;
        }
        drumStep = drumSteps_[instrumentID * DRUM_PATTERNS * STEPS_PER_PATTERN + pattern * STEPS_PER_PATTERN + step];
    }
    return true;
}
