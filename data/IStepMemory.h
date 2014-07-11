#ifndef ISEQUENCEMEMORY_H
#define ISEQUENCEMEMORY_H

#include "DrumStep.h"

class IStepMemory
{
public:
    virtual DrumStep getDrumStep( unsigned char instrumentID, unsigned char pattern, unsigned char step) = 0;
    virtual bool getNextActiveDrumStep( unsigned char instrumentID, unsigned char pattern, unsigned char & step, DrumStep & drumStep) = 0;
    virtual bool setDrumStep( unsigned char instrumentID, unsigned char pattern, unsigned char step, DrumStep stepData) = 0;
};


#endif // ISEQUENCEMEMORY_H
