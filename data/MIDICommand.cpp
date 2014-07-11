#include "MIDICommand.h"

MIDICommand::MIDICommand() : type_(NOTEOFF), note_(0), velocity_(0), channel_(0) {
}


MIDICommand::MIDICommand(MIDICommand::MIDICommandType type, unsigned char note, unsigned char velocity) :type_(type), note_(note), velocity_(velocity), channel_(0)
{

}
