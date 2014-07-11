#ifndef IMIDICOMMANDPROCESSOR_H
#define IMIDICOMMANDPROCESSOR_H

#include "../data/MIDICommand.h"

class IMIDICommandProcessor {
public:
    virtual void SendCommand(MIDICommand & command) = 0;
};

#endif // IMIDICOMMANDPROCESSOR_H
