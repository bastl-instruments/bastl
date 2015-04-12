/*
 * Arpeggiator_test.cpp
 *
 *  Created on: Apr 11, 2015
 *      Author: Martin Baar
 */

#include "../Arpeggiator.h"
#include <stdio.h>
#include <string>

bool testSimpleAdd() {
	printf("testSimpleAdd\n");
	Arpeggiator arp;
	bool returnValue = true;

	// Testing adding one item
	bool ok = arp.addNote(10);
	if (!ok) { printf("ERROR: Test failed adding first item\n"); returnValue = false;}

	//testing adding existing item
	ok = arp.addNote(10);
	if (ok) { printf("ERROR: Test failed adding existing item\n"); returnValue = false;}

	//filling values up to the limit
	for (unsigned char i = 1; i < ARPEGGIATOR_MAX_VALUES; i++) {
		ok = arp.addNote(10 + i);
		if (!ok) { printf("ERROR: Test failed adding item number: %d\n", i + 1); returnValue = false;}
	}

	ok = arp.addNote(1);
	if (ok) { printf("ERROR: Test failed adding item over limit\n"); returnValue = false;}

	//test add after removing one
	ok = arp.removeNote(11);
	if (!ok) { printf("ERROR: Test failed preparing test data by deleting item\n"); return false;}

	ok = arp.addNote(1);
	if (!ok) { printf("ERROR: Test failed adding item over after deleting one\n"); returnValue = false;}


	return returnValue;
}

bool testSimpleRemove() {
	printf("testSimpleRemove\n");
	Arpeggiator arp;
	bool returnValue = true;

	for (int i = 0; i < ARPEGGIATOR_MAX_VALUES; i++) {
		arp.addNote(i);
	}

	for (int i = 0; i < ARPEGGIATOR_MAX_VALUES; i++) {
		bool ok = arp.removeNote((i + 5) % 20);
		if (!ok) { printf("ERROR: Test failed removing existing item of value %d \n", (i + 5) % 20); returnValue = false;}
	}

	bool ok = arp.removeNote(20);
	if (ok) { printf("ERROR: Test failed removing not-existing item\n"); returnValue = false;}

	return returnValue;
}

std::string arpeggioAlgorithmToString(Arpeggiator::ArpeggioAlgorithm algorithm) {
	switch (algorithm) {
		case Arpeggiator::UP :
			return "UP";
		case Arpeggiator::DOWN:
			return "DOWN";
		case Arpeggiator::UP_DOWN:
			return "UP_DOWN";
		case Arpeggiator::SORTED:
			return "SORTED";
		case Arpeggiator::RANDOM:
			return "RANDOM";
		default:
			return "UNKNOWN";
	}
	return "UNKNOWN";
}

bool testAlgorithm(Arpeggiator::ArpeggioAlgorithm algorithm,
				   unsigned char windowSize,
				   unsigned char windowSizeWide,
				   unsigned char * dataToAdd,
				   unsigned char initialItemCount,
				   unsigned char totalItemCount,
				   unsigned char itemToRemove,
				   unsigned char testArraySize,
				   unsigned char * dataShort,
				   unsigned char * dataLong,
				   unsigned char * dataWideWindow,
				   unsigned char * dataAfterRemove) {

	Arpeggiator arp;
	arp.setWindowSize(windowSize);
	arp.setAlgorithm(algorithm);
	bool returnValue = true;
	std::string algorithmName = arpeggioAlgorithmToString(algorithm);

	//Test we do not get any value when array is Empty
	unsigned char value = 0;
	bool ok = arp.getNextNote(value);
	if (ok) { printf("ERROR: Test GET (%s) failed getting note without adding any\n", algorithmName.c_str()); returnValue = false;}

	//Only limited number of items in arpeggiator
	for (unsigned char i = 0; i < initialItemCount; i++) {
		bool ok = arp.addNote(dataToAdd[i]);
		if (!ok) { printf("ERROR: Test ADD/GET (%s) failed preparing test data filling array with note: %d \n", algorithmName.c_str(), (i + 1)); return false;}
	}
	for (unsigned char i = 0; i < testArraySize; i++) {
		bool ok = arp.getNextNote(value);
	    if (!ok) { printf("ERROR: Test ADD/GET (%s) failed getting note number: %d when only %d in the sequence \n", algorithmName.c_str(), (i + 1), initialItemCount); returnValue = false;}
	    if (value != dataShort[i]) { printf("ERROR: Test ADD/GET (%s) getting wrong note for seq. number: %d when only %d in the sequence. expected: %d actual: %d  \n",
	    							 algorithmName.c_str(), (i + 1), initialItemCount, dataShort[i], value); returnValue = false;}
	}

	//All items in arpeggiator
	for (unsigned char i = initialItemCount; i < totalItemCount; i++) {
		bool ok = arp.addNote(dataToAdd[i]);
		if (!ok) { printf("ERROR: Test ADD/GET (%s) failed preparing test data filling array with note: %d \n", algorithmName.c_str(), (i + 1)); return false;}
	}

	for (unsigned char i = 0; i < testArraySize; i++) {
		bool ok = arp.getNextNote(value);
		if (!ok) {printf("ERROR: Test ADD/GET (%s) failed getting note number: %d \n", algorithmName.c_str(), (i + 1)); returnValue = false; }
		if (value != dataLong[i]) { printf("ERROR: Test ADD/GET (%s) getting wrong note for seq. number: %d expected: %d actual: %d  \n", algorithmName.c_str(), (i + 1), dataLong[i], value); returnValue = false;}
	}

	//After deleting item
	ok = arp.removeNote(itemToRemove);
	arp.reset();
	if (!ok) { printf("ERROR: Test REMOVE/GET (%s) failed preparing test data removing note: %d \n", algorithmName.c_str(), itemToRemove); return false;}
	for (unsigned char i = 0; i < testArraySize; i++) {
		bool ok = arp.getNextNote(value);
		if (!ok) {printf("ERROR: Test REMOVE/GET (%s) failed getting note number: %d after remove \n", algorithmName.c_str(), (i + 1)); returnValue = false; }
		if (value != dataAfterRemove[i]) { printf("ERROR: Test REMOVE/GET (%s) getting wrong note for seq. number: %d after remove expected: %d actual: %d  \n", algorithmName.c_str(), (i + 1), dataAfterRemove[i], value); returnValue = false;}
	}

	//Testing reset
	arp.reset();
	for (unsigned char i = 0; i < (windowSize - 1); i++) {
		bool ok = arp.getNextNote(value);
		if (!ok) {printf("ERROR: Test RESET/GET (%s) failed getting note number: %d after remove and first reset \n", algorithmName.c_str(), (i + 1)); returnValue = false; }
		if (value != dataAfterRemove[i]) { printf("ERROR: Test RESET/GET (%s) getting wrong note for seq. number: %d after remove and first reset expected: %d actual: %d  \n", algorithmName.c_str(), (i + 1), dataAfterRemove[i], value); returnValue = false;}
	}
	arp.reset();
	for (unsigned char i = 0; i < (windowSize - 1); i++) {
		bool ok = arp.getNextNote(value);
		if (!ok) {printf("ERROR: Test RESET/GET (%s) failed getting note number: %d after real reset \n", algorithmName.c_str(), (i + 1)); returnValue = false; }
		if (value != dataAfterRemove[i]) { printf("ERROR: Test RESET/GET (%s) getting wrong note for seq. number: %d after real reset: %d actual: %d  \n", algorithmName.c_str(), (i + 1), dataAfterRemove[i], value); returnValue = false;}
	}

	//Testing clear
	arp.clear();
	ok = arp.getNextNote(value);
	if (ok) { printf("ERROR: Test CLEAR/GET (%s) failed getting note after clear\n", algorithmName.c_str()); returnValue = false;}

	//Add items in arpeggiator after clear
	for (unsigned char i = 0; i < totalItemCount; i++) {
		bool ok = arp.addNote(dataToAdd[i]);
		if (!ok) { printf("ERROR: Test CLEAR/ADD/GET (%s) failed preparing test data filling array with note: %d \n", algorithmName.c_str(), (i + 1)); return false;}
	}

	for (unsigned char i = 0; i < testArraySize; i++) {
		bool ok = arp.getNextNote(value);
		if (!ok) {printf("ERROR: Test CLEAR/ADD/GET (%s) failed getting note number: %d \n", algorithmName.c_str(), (i + 1)); returnValue = false; }
		if (value != dataLong[i]) { printf("ERROR: CLEAR/ADD/GET Test (%s) getting wrong note for seq. number: %d expected: %d actual: %d  \n", algorithmName.c_str(), (i + 1), dataLong[i], value); returnValue = false;}
	}

	//Testing changed window size
	arp.setWindowSize(windowSizeWide);
	arp.reset();
	for (unsigned char i = 0; i < testArraySize; i++) {
		bool ok = arp.getNextNote(value);
		if (!ok) {printf("ERROR: Test CLEAR/ADD/GET (%s) failed getting note number: %d \n", algorithmName.c_str(), (i + 1)); returnValue = false; }
		if (value != dataWideWindow[i]) { printf("ERROR: CLEAR/ADD/GET Test (%s) getting wrong note for seq. number: %d expected: %d actual: %d  \n", algorithmName.c_str(), (i + 1), dataWideWindow[i], value); returnValue = false;}
	}
	return returnValue;
}

bool testUpAlgorithm() {
	printf("testUpAlgorithm\n");
	unsigned char dataToAdd[6] = {6,5,4,3,2,1};
	unsigned char dataShort[15] = {6,5,6,5,6,5,6,5,6,5,6,5,6,5,6};
	unsigned char dataLong[15] = {3,2,1,3,2,1,3,2,1,3,2,1,3,2,1};
	unsigned char dataWiderWindow[15] = {4,3,2,1,4,3,2,1,4,3,2,1,4,3,2};
	unsigned char dataAfterRemove[15] = {4,3,1,4,3,1,4,3,1,4,3,1,4,3,1};
	return testAlgorithm(Arpeggiator::UP, 3, 4, dataToAdd,
						  2, 6, 2, 15,
						  dataShort, dataLong, dataWiderWindow, dataAfterRemove);
}

bool testDownAlgorithm() {
	printf("testDownAlgorithm\n");
	unsigned char dataToAdd[6] = 		{6,5,4,3,2,1};
	unsigned char dataShort[15] = 		{5,6,5,6,5,6,5,6,5,6,5,6,5,6,5};
	unsigned char dataLong[15] =  		{1,2,3,1,2,3,1,2,3,1,2,3,1,2,3};
	unsigned char dataWiderWindow[15] =	{1,2,3,4,1,2,3,4,1,2,3,4,1,2,3};
	unsigned char dataAfterRemove[15] =	{1,3,4,1,3,4,1,3,4,1,3,4,1,3,4};
	return testAlgorithm(Arpeggiator::DOWN, 3, 4, dataToAdd,
						  2, 6, 2, 15,
						  dataShort, dataLong, dataWiderWindow, dataAfterRemove);
}

bool testUpDownAlgorithm() {
	printf("testUpDownAlgorithm\n");
	unsigned char dataToAdd[6] = 			{6,5,4,3,2,1};
		unsigned char dataShort[15] = 		{6,5,6,5,6,5,6,5,6,5,6,5,6,5,6};
		unsigned char dataLong[15] = 		{3,2,1,2,3,2,1,2,3,2,1,2,3,2,1};
		unsigned char dataWiderWindow[15] = {4,3,2,1,2,3,4,3,2,1,2,3,4,3,2};
		unsigned char dataAfterRemove[15] = {4,3,1,3,4,3,1,3,4,3,1,3,4,3,1};
	return testAlgorithm(Arpeggiator::UP_DOWN, 3, 4, dataToAdd,
						 2, 6, 2, 15,
	  				     dataShort, dataLong, dataWiderWindow, dataAfterRemove);return false;
}

bool testSortedAlgorithm() {
	printf("testSortedAlgorithm\n");
	unsigned char dataToAdd[6] = 		{6,5,4,3,2,1};
	unsigned char dataShort[15] = 		{5,6,5,6,5,6,5,6,5,6,5,6,5,6,5};
	unsigned char dataLong[15] = 		{1,2,3,1,2,3,1,2,3,1,2,3,1,2,3};
	unsigned char dataWiderWindow[15] = {1,2,3,4,1,2,3,4,1,2,3,4,1,2,3};
	unsigned char dataAfterRemove[15] = {1,3,4,1,3,4,1,3,4,1,3,4,1,3,4};
	return testAlgorithm(Arpeggiator::SORTED, 3, 4, dataToAdd,
						 2, 6, 2, 15,
		  				 dataShort, dataLong, dataWiderWindow, dataAfterRemove);return false;return false;
}

bool testRandomAlgorithm() {
	printf("testRandomAlgorithm\n");
	Arpeggiator arp;
	arp.setWindowSize(4);
	arp.setAlgorithm(Arpeggiator::RANDOM);

	bool returnValue = true;

	unsigned char dataToAdd[7] = {10,6,5,4,3,2,1};
	for (unsigned char i = 0; i < 7; i++) {
		bool ok = arp.addNote(dataToAdd[i]);
		if (!ok) { printf("ERROR: Test CLEAR/ADD/GET (RANDON) failed preparing test data filling array with note: %d \n", (i + 1)); return false;}
	}
	for (unsigned char i = 0; i < 30; i++) {
		unsigned char value;
		bool ok = arp.getNextNote(value);
		if (!ok) {printf("ERROR: Test GET (RANDOM) failed getting note number: %d \n", (i + 1)); returnValue = false; }
		if (value == 0 || value > 4) { printf("ERROR: GET Test (RANDOM) getting wrong note for seq. number: %d expected: 1 ..4 actual: %d  \n", (i + 1), value); returnValue = false;}
	}
	return returnValue;
}

bool testPlayAndAddNote(Arpeggiator::ArpeggioAlgorithm algorithm, unsigned char * testedNotes, unsigned char * expectedData, unsigned char noteCount, unsigned char expectedDataCount) {
	std::string algorithmName = arpeggioAlgorithmToString(algorithm);
	printf("testAddingWhilePlayingNote (%s)\n", algorithmName.c_str());
	bool returnValue = true;
	Arpeggiator arp;
	arp.setWindowSize(3);
	arp.setAlgorithm(algorithm);
	for (unsigned char i = 0; i < noteCount - 2; i++) {
		bool ok = arp.addNote(testedNotes[i]);
		if (!ok) { printf("ERROR: Test ADD failed preparing test data filling array with note: %d \n", (i + 1)); return false;}
	}

	unsigned char note;
	arp.getNextNote(note);

	for (unsigned char i = noteCount - 2; i < noteCount; i++) {
		bool ok = arp.addNote(testedNotes[i]);
		if (!ok) { printf("ERROR: Test ADD failed preparing test data filling array with note: %d \n", (i + 1)); return false;}
	}

	for (unsigned char i = 0; i < expectedDataCount; i++) {
		unsigned char value;
		bool ok = arp.getNextNote(value);
		if (!ok) {printf("ERROR: Test GET/ADD_IN_PLAY (%s) failed getting note number: %d \n", algorithmName.c_str(), (i + 1)); returnValue = false; }
		if (value != expectedData[i]) { printf("ERROR: GET/ADD_IN_PLAY Test (%s) getting wrong note for seq. number: %d expected: %d actual: %d  \n", algorithmName.c_str(), (i + 1), expectedData[i], value); returnValue = false;}
	}
	//Going to tested step;
	return returnValue;
}

bool testRemoveBeforeNote(Arpeggiator::ArpeggioAlgorithm algorithm, unsigned char * testedNotes, unsigned char * expectedData, unsigned char noteCount, unsigned char expectedDataCount, unsigned char noteToRemove) {
	std::string algorithmName = arpeggioAlgorithmToString(algorithm);
	printf("testRemovingPlayedNote (%s)\n", algorithmName.c_str());
	bool returnValue = true;
	Arpeggiator arp;
	arp.setWindowSize(3);
	arp.setAlgorithm(algorithm);
	for (unsigned char i = 0; i < noteCount; i++) {
		bool ok = arp.addNote(testedNotes[i]);
		if (!ok) { printf("ERROR: Test ADD failed preparing test data filling array with note: %d \n", (i + 1)); return false;}
	}

	unsigned char note;
	arp.getNextNote(note);
	arp.removeNote(noteToRemove);

	for (unsigned char i = 0; i < expectedDataCount; i++) {
		unsigned char value;
		bool ok = arp.getNextNote(value);
		if (!ok) {printf("ERROR: Test GET/REMOVE_IN_PLAY (%s) failed getting note number: %d \n", algorithmName.c_str(), (i + 1)); returnValue = false; }
		if (value != expectedData[i]) { printf("ERROR: GET/REMOVE_IN_PLAY Test (%s) getting wrong note for seq. number: %d expected: %d actual: %d  \n", algorithmName.c_str(), (i + 1), expectedData[i], value); returnValue = false;}
	}
	//Going to tested step;
	return returnValue;
}

bool testAddWithPlayingNote() {
	bool returnValue = true;
	unsigned char dataAdd[8] = {13,12,11,10,9,6,7,8};
	unsigned char data1[15] = {6,7,8,6,7,8,6,7,8,6,7,8,6,7,8};
	returnValue = returnValue && testPlayAndAddNote(Arpeggiator::UP, dataAdd, data1, 8, 15);
	unsigned char data2[15] = {8,7,6,8,7,6,8,7,6,8,7,6,8,7,6};
	returnValue = returnValue && testPlayAndAddNote(Arpeggiator::DOWN, dataAdd, data2, 8, 15);
	unsigned char data3[15] = {6,7,8,7,6,7,8,7,6,7,8,7,6,7,8};
	returnValue = returnValue && testPlayAndAddNote(Arpeggiator::UP_DOWN, dataAdd, data3, 8, 15);
	unsigned char data4[15] = {7,8,6,7,8,6,7,8,6,7,8,6,7,8,6};
	returnValue = returnValue && testPlayAndAddNote(Arpeggiator::SORTED, dataAdd, data4, 8, 15);
	return returnValue;
}

bool testRemoveBeforePlayingNote() {
	printf("testRemoveBefore\n");
	bool returnValue = true;
	unsigned char dataAdd[6] = {13,15,11,10,9,6};
	unsigned char data1[15] = {9,6,10,9,6,10,9,6,10,9,6,10,9,6,10};
	returnValue = returnValue && testRemoveBeforeNote(Arpeggiator::UP, dataAdd, data1, 6, 15, 11);
	unsigned char data2[15] = {9,10,6,9,10,6,9,10,6,9,10,6,9,10,6};
	returnValue = returnValue && testRemoveBeforeNote(Arpeggiator::DOWN, dataAdd, data2, 6, 15, 11);
	unsigned char data3[15] = {9,6,9,10,9,6,9,10,9,6,9,10,9,6,9};
	returnValue = returnValue && testRemoveBeforeNote(Arpeggiator::UP_DOWN, dataAdd, data3, 6, 15, 11);
	unsigned char data4[15] = {9,10,6,9,10,6,9,10,6,9,10,6,9,10,6};
	returnValue = returnValue && testRemoveBeforeNote(Arpeggiator::SORTED, dataAdd, data4, 6, 15, 11);
	return returnValue;
}

bool testRemoveAfterPlayingNote() {
	printf("testRemoveAfter\n");
	bool returnValue = true;
	unsigned char dataAdd[6] = {13,15,11,10,9,6};
	unsigned char data1[15] = {6,11,10,6,11,10,6,11,10,6,11,10,6,11,10};
	returnValue = returnValue && testRemoveBeforeNote(Arpeggiator::UP, dataAdd, data1, 6, 15, 9);
	unsigned char data2[15] = {10,11,6,10,11,6,10,11,6,10,11,6,10,11,6};
	returnValue = returnValue && testRemoveBeforeNote(Arpeggiator::DOWN, dataAdd, data2, 6, 15, 9);
	unsigned char data3[15] = {6,10,11,10,6,10,11,10,6,10,11,10,6,10,11};
	returnValue = returnValue && testRemoveBeforeNote(Arpeggiator::UP_DOWN, dataAdd, data3, 6, 15, 9);
	unsigned char data4[15] = {10,11,6,10,11,6,10,11,6,10,11,6,10,11,6};
	returnValue = returnValue && testRemoveBeforeNote(Arpeggiator::SORTED, dataAdd, data4, 6, 15, 9);
	return returnValue;
}

bool testRemovePlayingNote() {
	printf("testRemovePlaying\n");
	bool returnValue = true;
	unsigned char dataAdd[6] = {13,15,11,10,9,6};
	unsigned char data1[15] = {9,6,11,9,6,11,9,6,11,9,6,11,9,6,11};
	returnValue = returnValue && testRemoveBeforeNote(Arpeggiator::UP, dataAdd, data1, 6, 15, 10);
	unsigned char data2[15] = {9,10,11,9,10,11,9,10,11,9,10,11,9,10,11};
	returnValue = returnValue && testRemoveBeforeNote(Arpeggiator::DOWN, dataAdd, data2, 6, 15, 6);
	unsigned char data3[15] = {9,6,9,11,9,6,9,11,9,6,9,11,9,6,9};
	returnValue = returnValue && testRemoveBeforeNote(Arpeggiator::UP_DOWN, dataAdd, data3, 6, 15, 10);
	unsigned char data4[15] = {9,10,11,9,10,11,9,10,11,9,10,11,9,10,11};
	returnValue = returnValue && testRemoveBeforeNote(Arpeggiator::SORTED, dataAdd, data4, 6, 15, 6);
	return returnValue;
}

int main( int argc, const char* argv[] ) {
	bool testResult = true;
	testResult = testResult && testSimpleAdd();
	testResult = testResult && testSimpleRemove();
	testResult = testResult && testUpAlgorithm();
	testResult = testResult && testDownAlgorithm();
	testResult = testResult && testUpDownAlgorithm();
	testResult = testResult && testSortedAlgorithm();
	testResult = testResult && testRandomAlgorithm();
	testResult = testResult && testAddWithPlayingNote();
	testResult = testResult && testRemoveBeforePlayingNote();
	testResult = testResult && testRemoveAfterPlayingNote();
	testResult = testResult && testRemovePlayingNote();
	return testResult ? 0 : 1;
}

