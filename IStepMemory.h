#ifndef ISEQUENCEMEMORY_H
#define ISEQUENCEMEMORY_H

#include "DrumStep.h"

class IStepMemory
{
public:
	enum ActiveMultiStatus{ALLACTIVE, ALLINACTIVE, MIXED};
    virtual DrumStep getDrumStep( unsigned char instrumentID, unsigned char step) = 0;
    virtual bool getNextActiveDrumStep( unsigned char instrumentID, unsigned char & step, DrumStep & drumStep) = 0;
    virtual bool setDrumStep( unsigned char instrumentID, unsigned char step, DrumStep stepData) = 0;
    virtual void getActivesAndMutesForNote(unsigned char instrumentID, unsigned char windowIndex, unsigned char * data) = 0;
    virtual void setDataReference(unsigned char * dataReference) = 0;
    virtual void getAllInstrumentActivesFor16Steps(unsigned char fromIndex, ActiveMultiStatus * result) = 0;
    virtual void getActiveWindowBitArray(unsigned char instrument, bool * result) = 0;
    virtual void getAllInstrumentsActiveWindowBitArray(bool * result) = 0;
    virtual void makeActiveUpTo(unsigned char instrument, unsigned char indexUpTo) = 0;
    virtual void makeAllInstrumentsActiveUpTo(unsigned char indexUpTo) = 0;
    virtual void clearStepsForInstrument(unsigned char instrument) = 0;
    virtual void clearStepsForAllInstruments() = 0;
    virtual bool isInDefaultState(unsigned char instrument) = 0;
    virtual void copyPan(unsigned char instrumentID, unsigned char panFrom, unsigned char panTo) = 0;
};


#endif // ISEQUENCEMEMORY_H
