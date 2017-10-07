#ifndef ILEDSANDBUTTONSHW_H
#define ILEDSANDBUTTONSHW_H

#include <inttypes.h>
#include "ILEDHW.h"
#include "IButtonHW.h"

class ILEDsAndButtonsHW : public ILEDHW, public IButtonHW {
public:

	enum TriggerState{GATE_ON, GATE_OFF, TRIGGER_ON};

	virtual uint16_t getElapsedBastlCycles() = 0;
	virtual void setTrigger(uint8_t number, bool state,bool autoOff) = 0;
};

#endif // ILEDSANDBUTTONSHW_H
