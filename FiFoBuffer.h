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

	FiFoBuffer():
		head(0),
		tail(0),
		fillCount(0)
		{}

	// Add a new entry to the buffer
	// returns 1 on success and 0 on full buffer
	bool add(const type&);

	// Get the oldest entry in the buffer
	// returns 1 on success and 0 on empty buffer
	bool get(type&);

	// Read the oldest value without deleting it from the buffer
	// returns 1 on success and 0 on empty buffer
	bool peek(type&);

	// Check if buffer is full
	bool isFull();

	// Check if buffer is empty
	bool isEmpty();

	// get the number of entries in the buffer
	uint8_t getfillCount() { return fillCount;}

	// get direct Pointer to ring Buffer
	type* getPointer() {return buffer;}

private:
	uint8_t head;
	uint8_t tail;
	uint8_t fillCount;

	type buffer[size];

	// increment position with wrapping on end of buffer
	void incrementPos(uint8_t& pos);

};

template <unsigned char size, typename type>
void FiFoBuffer<size,type>::incrementPos(uint8_t& pos) {
	pos++;
	if (pos == size) pos = 0;
}

template <unsigned char size, typename type>
bool FiFoBuffer<size,type>::add(const type& val) {

	if (isFull()) return false;

	buffer[head] = val;
	incrementPos(head);
	fillCount++;

	return true;
}

template <unsigned char size, typename type>
bool FiFoBuffer<size,type>::get(type& val) {

	if (isEmpty()) return false;

	val = buffer[tail];
	incrementPos(tail);
	fillCount--;

	return true;
}

template <unsigned char size, typename type>
bool FiFoBuffer<size,type>::peek(type& val) {

	if (isEmpty()) return false;

	val = buffer[tail];
	return true;
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
