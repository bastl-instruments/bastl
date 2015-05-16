#ifndef ILEDSANDBUTTONSHW_H
#define ILEDSANDBUTTONSHW_H

#include <inttypes.h>
#include "ILEDHW.h"
#include "IButtonHW.h"

class ILEDsAndButtonsHW : public ILEDHW, public IButtonHW {
public:

	enum TriggerState{GATE_ON, GATE_OFF, TRIGGER_ON};

	virtual unsigned int getElapsedBastlCycles() = 0;
	virtual void setTrigger(uint8_t number, bool state,uint8_t length) = 0;
};

#endif // ILEDSANDBUTTONSHW_H
