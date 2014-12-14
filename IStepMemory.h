#ifndef ISEQUENCEMEMORY_H
#define ISEQUENCEMEMORY_H

#include "DrumStep.h"

class IStepMemory
{
public:
    virtual DrumStep getDrumStep( unsigned char instrumentID, unsigned char step) = 0;
    virtual bool getNextActiveDrumStep( unsigned char instrumentID, unsigned char & step, DrumStep & drumStep) = 0;
    virtual bool setDrumStep( unsigned char instrumentID, unsigned char step, DrumStep stepData) = 0;
    virtual void getActivesAndMutesForNote(unsigned char instrumentID, unsigned char windowIndex, unsigned char * data) = 0;
};


#endif // ISEQUENCEMEMORY_H
