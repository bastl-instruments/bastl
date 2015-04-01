/*
 * MIDInoteBuffer.cpp

 *
 *  Created on: Jan 28, 2015
 *      Author: dasvaclav
 */
#include "MIDInoteBuffer.h"
#include <basic.h>

#define LEFT_SHIFT 0
#define RIGHT_SHIFT 1
void MIDInoteBuffer::init(){
	clearBuffers();
}
void MIDInoteBuffer::orderBuffer(){
	 for(int j=0;j<BUFFER_SIZE;j++)  orderedBuffer[j]=0;
	  unsigned char nextOne=255;
	  for(int i=0;i<notesInBuffer;i++){
	    if(midiBuffer[i] < nextOne) nextOne=midiBuffer[i];
	  }
	  orderedBuffer[0]=nextOne;

	  for(int j=1;j<notesInBuffer;j++){
	    nextOne=255;
	    for(int i=0;i<notesInBuffer;i++){
	      if(midiBuffer[i]<nextOne && midiBuffer[i] > orderedBuffer[j-1] ) nextOne=midiBuffer[i];
	    }
	    orderedBuffer[j]=nextOne;
	  }
}
void MIDInoteBuffer::setPolyphony(uint8_t _polyphony){
	polyphony=_polyphony;
}
uint8_t MIDInoteBuffer::getNoteToBePlayedAgain(){
	return midiBuffer[polyphony];
}
uint8_t MIDInoteBuffer::getVelocityOfNoteToBePlayedAgain(){
	return velocityBuffer[polyphony];
}
uint8_t MIDInoteBuffer::getNumberOfNotesInBuffer(){
	return notesInBuffer;
}
uint8_t MIDInoteBuffer::getNoteFromBuffer(uint8_t _note){
	return midiBuffer[_note];
}
uint8_t MIDInoteBuffer::getVelocityFromBuffer(uint8_t _note){
	return velocityBuffer[_note];
}
uint8_t MIDInoteBuffer::getNoteFromOrderedBuffer(uint8_t _note){
	return orderedBuffer[_note];
}
void MIDInoteBuffer::clearBuffers(){
	for(int i=0;i<BUFFER_SIZE;i++){
		midiBuffer[i]=0;
		velocityBuffer[i]=0;
		orderedBuffer[i]=0;
	}
	notesInBuffer=0;
}
void  MIDInoteBuffer::shiftBuffer(uint8_t direction,uint8_t from){
	if(direction==LEFT_SHIFT){
		for(int i=from;i<notesInBuffer;i++){
			midiBuffer[i]=midiBuffer[i+1];
			velocityBuffer[i]=velocityBuffer[i+1];
		 }
	}
	else if(direction==RIGHT_SHIFT){
		  for(int i=notesInBuffer;i>from;i--){
			midiBuffer[i]=midiBuffer[i-1];
			velocityBuffer[i]=velocityBuffer[i-1];
		  }
	}
}
uint8_t  MIDInoteBuffer::findNoteInBuffer(uint8_t note){
	for(int i=0;i<notesInBuffer;i++){
		if(midiBuffer[i]==note) return i;
	}
	return 255;
}

bool MIDInoteBuffer::removeNoteFromBuffer(uint8_t note){
	if(notesInBuffer>0){
		uint8_t position=findNoteInBuffer(note);
		if(position!=255) shiftBuffer(LEFT_SHIFT,position),notesInBuffer--;
	}
	if(notesInBuffer>=polyphony) return 1;
	else return 0;
}
void MIDInoteBuffer::addNoteToBuffer(uint8_t _note, uint8_t _velocity){

	removeNoteFromBuffer(_note);
	shiftBuffer(RIGHT_SHIFT,0);

	if(notesInBuffer<BUFFER_SIZE){
		notesInBuffer++;
	}

	midiBuffer[0]=_note;
	velocityBuffer[0]=_velocity;
}
