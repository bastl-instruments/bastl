#ifndef ILEDSANDBUTTONSHW_H
#define ILEDSANDBUTTONSHW_H

#include <inttypes.h>
#include "ILEDHW.h"
#include "IButtonHW.h"

class ILEDsAndButtonsHW : public ILEDHW, public IButtonHW {
public:
	virtual unsigned int getElapsedBastlCycles() = 0;
};

#endif // ILEDSANDBUTTONSHW_H
