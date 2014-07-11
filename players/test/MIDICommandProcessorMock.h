#ifndef MIDICOMMANDPROCESSORMOCK_H
#define MIDICOMMANDPROCESSORMOCK_H

#include "../IMIDICommandProcessor.h"

class MIDICommandProcessorMock : public IMIDICommandProcessor
{
public:
    MIDICommandProcessorMock();
    virtual void SendCommand(MIDICommand & command);
    void clear();
    unsigned char getNoteOffCountForChannel(unsigned char channel);
    unsigned char getNoteOnCountForChannel(unsigned char channel);
private:
    unsigned char noteOnCount_[16];
    unsigned char noteOffCount_[16];
};

inline unsigned char MIDICommandProcessorMock::getNoteOffCountForChannel(unsigned char channel) {
    return noteOffCount_[channel];
}

inline unsigned char MIDICommandProcessorMock::getNoteOnCountForChannel(unsigned char channel) {
    return noteOnCount_[channel];
}

inline void MIDICommandProcessorMock::clear() {
    for (unsigned char i = 0; i < 16; i++) {
        noteOffCount_[i] = 0;
        noteOnCount_[i] = 0;
    }
}

#endif // MIDICOMMANDPROCESSORMOCK_H
