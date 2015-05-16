#ifndef ILEDSANDBUTTONSHW_H
#define ILEDSANDBUTTONSHW_H

#include <inttypes.h>
#include "ILEDHW.h"
#include "IButtonHW.h"

class ILEDsAndButtonsHW : public ILEDHW, public IButtonHW {
public:

	enum TriggerState{ON, OFF, TRIGGER};

	virtual unsigned int getElapsedBastlCycles() = 0;
	virtual void setTrigger(uint8_t number, TriggerState state) = 0;
};

#endif // ILEDSANDBUTTONSHW_H
