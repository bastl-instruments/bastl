/*
 * FiFoBuffer.h
 *
 *  Created on: 10.11.2014
 *      Author: user
 */

#ifndef FIFOBUFFER_H_
#define FIFOBUFFER_H_

#include <inttypes.h>

/*** FIFOBuffer ***
 * This class represents a first-in-first-out type buffer in ring structure
 * As template arguments you have to provide its size and the datatype you want to store
 */

template <unsigned char size, typename type>
class FiFoBuffer {
public:

	// The Nullobject will be returned after a read attempt on an empty buffer
	FiFoBuffer(type NullObject):
		head(0),
		tail(0),
		fillCount(0)
		{this->NullObject = NullObject;}

	// Add a new entry to the buffer
	// If the buffer is full, this function will silently return
	void add(type);

	// Get the oldest entry in the buffer
	// If the buffer is emtpy, this funtion will return the NullObject given in the constructor
	type get();

	// Read the oldest value without deleting it from the buffer
	// If the buffer is emtpy, this funtion will return the NullObject given in the constructor
	type peek();

	// Check if buffer is full
	bool isFull();

	// Check if buffer is empty
	bool isEmpty();

	// get the number of entries in the buffer
	uint8_t getfillCount() { return fillCount;}

private:
	uint8_t head;
	uint8_t tail;
	uint8_t fillCount;

	type buffer[size];
	type NullObject;

	void incrementPos(uint8_t& pos);

};

template <unsigned char size, typename type>
void incrementPos(uint8_t& pos) {
	pos++;
	if (pos == size) pos = 0;
}

template <unsigned char size, typename type>
void FiFoBuffer<size,type>::add(type val) {

	if (isFull()) return;

	buffer[head] = val;
	incrementPos(head);
	fillCount++;
}

template <unsigned char size, typename type>
type FiFoBuffer<size,type>::get() {

	if (isEmpty()) return NullObject;

	type returnVal = buffer[tail];
	incrementPos(tail);
	fillCount--;

	return returnVal;
}

template <unsigned char size, typename type>
type FiFoBuffer<size,type>::peek() {

	if (isEmpty()) return NullObject;

	return  buffer[tail];
}

template <unsigned char size, typename type>
bool FiFoBuffer<size,type>::isEmpty() {
	return (fillCount == 0);
}

template <unsigned char size, typename type>
bool FiFoBuffer<size,type>::isFull() {
	return (fillCount == size);
}



#endif /* FIFOBUFFER_H_ */
