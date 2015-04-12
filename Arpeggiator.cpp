/*
 * Arpeggiator.cpp
 *
 *  Created on: Apr 11, 2015
 *      Author: Martin Baar
 */

#include "Arpeggiator.h"
#include "random.h"

Arpeggiator::Arpeggiator() : algorithm_(Arpeggiator::UP), windowSize_(1), count_(0), started_(false),
							 currentIndex_(0), upDownGoingUp_(true), removeNoteAfterGettingNext_(false)
{
}

bool Arpeggiator::addNote(unsigned char note) {
	if (count_ != ARPEGGIATOR_MAX_VALUES) {
		unsigned char valueIndex;
		if (!findValue(note, notes_, valueIndex)) {
			notes_[count_] = note;
			count_++;
			fillSorted();
			return true;
		}
	}
	return false;
}

bool Arpeggiator::removeNote(unsigned char note) {

	//When note is just playing we shall remove it later when we want to do another step
	if (note == notes_[currentIndex_]) {
		removeNoteAfterGettingNext_ = true;
		return true;
	} else {
		unsigned char valueIndex;
		if (findValue(note, notes_, valueIndex)) {
			if (valueIndex < currentIndex_) {
				currentIndex_--;
			}
			for (unsigned char i = valueIndex; i < (count_ - 1); i++) {
				notes_[i] = notes_[i + 1];
			}
			count_--;
			fillSorted();
			return true;
		}
	}
	return false;
}

bool Arpeggiator::getNextNote(unsigned char& note) {
	if (count_ != 0) {
		unsigned char oldIndex = currentIndex_;
		do {
			currentIndex_ = getNextIndex();
		} while (currentIndex_ < (count_ - windowSize_));
		note = notes_[currentIndex_];
		if (removeNoteAfterGettingNext_) {
			removeNote(notes_[oldIndex]);
			removeNoteAfterGettingNext_ = false;
			if (count_ == 0) {
				return false;
			}
		}
		return true;
	}
	return false;
}

bool Arpeggiator::findValue(unsigned char value, unsigned char* array, unsigned char& index) {
	for (unsigned char i = 0; i < count_; i++) {
		if (array[i] == value) {
			index = i;
			return true;
		}
	}
	return false;
}

unsigned char Arpeggiator::getNextIndex() {
	if (started_) {
		switch (algorithm_) {
			case Arpeggiator::UP :
				return (currentIndex_ + 1) % count_;
			case Arpeggiator::DOWN:
				return (currentIndex_ == 0) ? (count_ - 1) : (currentIndex_ - 1);
			case Arpeggiator::UP_DOWN:
				unsigned char newIndex;
				if (upDownGoingUp_) {
					if ((currentIndex_ + 1) == count_) {
						upDownGoingUp_ = false;
						return currentIndex_ - 1;
					} else {
						return (currentIndex_ + 1) % count_;
					}
				} else {
					if (currentIndex_ == 0 || (currentIndex_ == count_ - windowSize_)) {
						upDownGoingUp_ = true;
						return (currentIndex_ + 1) % count_;
					} else {
						return currentIndex_ - 1;
					}
				}
				if (newIndex == (count_ - 1) || newIndex == (count_ - windowSize_) || newIndex == 0) {
					upDownGoingUp_ = ! upDownGoingUp_;
				}
				return newIndex;
			case Arpeggiator::SORTED:
				unsigned char indexInSorted;
				findValue(currentIndex_, sortedNotes_, indexInSorted);
				return sortedNotes_[(indexInSorted + 1) % count_];
			case Arpeggiator::RANDOM:
				return bastlRandom::range(count_- windowSize_, count_ - 1);
			default:
				return currentIndex_;
		}
	} else {

		//Sequence just started so based on used algorithm we have to return
		//correct first note to play
		started_ = true;

		//For Down initial index is last item not the first one!
		if (algorithm_ == Arpeggiator::DOWN) {
			currentIndex_ = (count_ - 1);
		} else if (algorithm_ == Arpeggiator::SORTED) {
			currentIndex_ = sortedNotes_[0];
		} else {
			currentIndex_ = 0;
			upDownGoingUp_ = true;
		}
	}
	return currentIndex_;
}

void Arpeggiator::fillSorted() {
	for (unsigned char sourceIndex = 0; sourceIndex < count_; sourceIndex++) {
		unsigned char value = notes_[sourceIndex];
		unsigned char targetIndex = 0;
		for (unsigned char checkedIndex = 0; checkedIndex < count_; checkedIndex++) {
			if (value > notes_[checkedIndex]) {
				targetIndex++;
			}
		}
		sortedNotes_[targetIndex] = sourceIndex;
	}
}
