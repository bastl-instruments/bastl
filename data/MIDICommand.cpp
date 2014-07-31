#include "MIDICommand.h"

MIDICommand::MIDICommand() : type_(NOTEOFF), channel_(0), note_(0), velocity_(0) {
}


MIDICommand::MIDICommand(MIDICommand::MIDICommandType type,
						unsigned char channel,
						unsigned char note,
						unsigned char velocity) :
								type_(type), channel_(channel), note_(note), velocity_(velocity)
{

}
