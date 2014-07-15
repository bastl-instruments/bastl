#ifndef DUMMYSEQUENCEMEMORY_H
#define DUMMYSEQUENCEMEMORY_H

#include "../IStepMemory.h"
#include "../InstrumentDefinitions.h"

class DummyStepMemory : public IStepMemory
{
public:

    DummyStepMemory();
    virtual DrumStep getDrumStep(unsigned char instrumentID, unsigned char pattern, unsigned char step);
    virtual bool setDrumStep(unsigned char instrumentID, unsigned char pattern, unsigned char step, DrumStep stepData);
    virtual bool getNextActiveDrumStep(unsigned char instrumentID, unsigned char pattern, unsigned char & step, DrumStep & drumStep) ;
private:
    DrumStep drumSteps_[DRUM_STEPS];
};

#endif // DUMMYSEQUENCEMEMORY_H
