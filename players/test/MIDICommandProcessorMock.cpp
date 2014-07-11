#include "MIDICommandProcessorMock.h"

MIDICommandProcessorMock::MIDICommandProcessorMock()
{
    clear();
}

void MIDICommandProcessorMock::SendCommand(MIDICommand &command)
{
    if (command.getType() == MIDICommand::NOTEOFF) {
        noteOffCount_[command.getChannel()]++;
    } else {
        noteOnCount_[command.getChannel()]++;
    }
}
