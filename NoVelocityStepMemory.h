#ifndef FLASHSTEPMEMORY_H
#define FLASHSTEPMEMORY_H

#include "IHWLayer.h"
#include "IStepMemory.h"
#include "DrumStep.h"
#include "RackInstrumentDefinitions.h"

/*
 * -----------------------------------------------------------------------
 * |                             PATTERN                           | ... |
 * -----------------------------------------------------------------------
 * |        INSTRUMENT1         |      INSTRUMENT 2          | ... | ... |
 * -----------------------------------------------------------------------
 * | ACTIVES | MUTES | SUBSTEPS | ACTIVES | MUTES | SUBSTEPS | ... | ... |
 * -----------------------------------------------------------------------
 *
 */

class NoVelocityStepMemory : public IStepMemory
{
public:
	NoVelocityStepMemory();
	virtual ~NoVelocityStepMemory();
    virtual DrumStep getDrumStep( unsigned char instrumentID, unsigned char step);
    virtual bool getNextActiveDrumStep( unsigned char instrumentID, unsigned char & step, DrumStep & drumStep);
    virtual bool setDrumStep(unsigned char instrumentID, unsigned char step, DrumStep stepData);
    virtual void getActivesAndMutesForNote(unsigned char instrumentID, unsigned char windowIndex, unsigned char * data);
    virtual void setDataReference(unsigned char * dataReference);
    virtual void getAllInstrumentActivesFor16Steps(unsigned char fromIndex, ActiveMultiStatus * result);
    virtual void getActiveWindowBitArray(unsigned char instrument, bool * result);
    virtual void getAllInstrumentsActiveWindowBitArray(bool * result);
    virtual void makeActiveUpTo(unsigned char instrument, unsigned char indexUpTo);
    virtual void makeAllInstrumentsActiveUpTo(unsigned char indexUpTo);
    virtual void clearStepsForInstrument(unsigned char instrument);
    virtual void clearStepsForAllInstruments();
    virtual bool isInDefaultState(unsigned char instrument);
    virtual void copyPan(unsigned char instrumentID, unsigned char panFrom, unsigned char panTo);
private:
    unsigned char * data_;
    void getDrumStepDataPointers(unsigned char instrumentID, unsigned char step,
    										   unsigned char *& mutes, unsigned char *& actives,
    										   unsigned char *& data);
    unsigned int getInstrumentDataOffset(unsigned char instrumentID);
};

inline void NoVelocityStepMemory::setDataReference(unsigned char * dataReference) {
	data_ = dataReference;
}


inline unsigned int NoVelocityStepMemory::getInstrumentDataOffset(unsigned char instrumentID) {
	// 64 steps, 6 bits per step = 384 bits = 48 bytes
	return (unsigned int)instrumentID * 48;
}

#endif // FLASHSTEPMEMORY_H
