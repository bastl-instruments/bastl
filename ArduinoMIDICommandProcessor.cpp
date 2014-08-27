/*
 * ArduinoMIDICommandProcessor.cpp
 *
 *  Created on: Jul 17, 2014
 *      Author: Martin Baar
 */

#include "ArduinoMIDICommandProcessor.h"

ArduinoMIDICommandProcessor::ArduinoMIDICommandProcessor(void (*sendNoteOnCallback)(unsigned char note,
															unsigned char velocity,
															unsigned char channel),
								void (*sendNoteOffCallback)(unsigned char note,
															 unsigned char velocity,
															 unsigned char channel)) :
															 sendNoteOnCallback_(sendNoteOnCallback),
															 sendNoteOffCallback_(sendNoteOffCallback){
}

void ArduinoMIDICommandProcessor::SendCommand(MIDICommand & command) {
	if (command.getType() == MIDICommand::NOTEON) {
		if (sendNoteOnCallback_ != 0) {
			sendNoteOnCallback_(command.getNote(), command.getVelocity(), command.getChannel());
		}
	} else {
		if (sendNoteOffCallback_ != 0) {
			sendNoteOffCallback_(command.getNote(), command.getVelocity(), command.getChannel());
		}
	}

}

