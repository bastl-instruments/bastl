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
    virtual DrumStep getDrumStep( unsigned char instrumentID, unsigned char step);
    virtual bool getNextActiveDrumStep( unsigned char instrumentID, unsigned char & step, DrumStep & drumStep);
    virtual bool setDrumStep(unsigned char instrumentID, unsigned char step, DrumStep stepData);
    virtual void getActivesAndMutesForNote(unsigned char instrumentID, unsigned char windowIndex, unsigned char * data);
    virtual void setDataReference(unsigned char * dataReference);
    virtual void getAllInstrumentActivesFor16Steps(unsigned char windowIndex, ActiveMultiStatus * result);
    virtual void getActiveWindowBitArray(unsigned char instrument, bool * result);
    virtual void getAllInstrumentsActiveWindowBitArray(bool * result);
    virtual void makeActiveUpTo(unsigned char instrument, unsigned char indexUpTo);
    virtual void makeAllInstrumentsActiveUpTo(unsigned char indexUpTo);
private:
    unsigned char * data_;
};

inline void NoVelocityStepMemory::setDataReference(unsigned char * dataReference) {
	data_ = dataReference;
}

#endif // FLASHSTEPMEMORY_H
