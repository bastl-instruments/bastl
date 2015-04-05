
#ifndef MIDI_NOTE_BUFFER_H_
#define MIDI_NOTE_BUFFER_H_

#include <basic.h>
#define BUFFER_SIZE 64
class MIDInoteBuffer {

public:
	void init();
	void addNoteToBuffer(uint8_t _note, uint8_t _velocity);
	bool removeNoteFromBuffer(uint8_t note);
	void orderBuffer();
	void setPolyphony(uint8_t _polyphony);
	uint8_t getNoteToBePlayedAgain();
	uint8_t getVelocityOfNoteToBePlayedAgain();
	uint8_t getNumberOfNotesInBuffer();
	uint8_t getNoteFromBuffer(uint8_t _note);
	uint8_t getVelocityFromBuffer(uint8_t _note);
	uint8_t getNoteFromOrderedBuffer(uint8_t _note);
	void clearBuffers();



private:
	uint8_t polyphony;
	uint8_t notesInBuffer;
	uint8_t midiBuffer[BUFFER_SIZE];
	uint8_t velocityBuffer[BUFFER_SIZE];
	uint8_t orderedBuffer[BUFFER_SIZE];
	void shiftBuffer(uint8_t direction,uint8_t from);
	uint8_t findNoteInBuffer(uint8_t note);

};





#endif
