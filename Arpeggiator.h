/*
 * Arpeggiator.h
 *
 *  Created on: Apr 11, 2015
 *      Author: Martin Baar
 */

#ifndef ARPEGGIATOR_H_
#define ARPEGGIATOR_H_

#define ARPEGGIATOR_MAX_VALUES 20

/**
 * Class to generate arpeggiato from set of notes
 */
class Arpeggiator {

public:

	/** Enumeration of possible arpeggio flows  */
	enum ArpeggioAlgorithm {UP, DOWN, UP_DOWN, SORTED, RANDOM};

	/** Default constructor */
	Arpeggiator();

	/**
	 * Method to add note to arpeggiator
	 * @param note note to add
	 * @return true if add successfully otherwise false (buffer full or note already there)
	 */
	bool addNote(unsigned char note);

	/**
	 * Method to remove note from arpeggiator
	 * @param note note to remove
	 * @return true if removed successfully otherwise false (note not there)
	 */
	bool removeNote(unsigned char note);

	/**
	 * To get next note to play
	 * @param note return argument filled with next note to play if any exists otherwise kept unchanged
	 * @return true if there is some note to be played as next
	 */
	bool getNextNote(unsigned char & note);

	//Setters

	/**
	 * Sets the algorithm arpeggiator shall use to generate sequence
	 * @param algorithm algorithm to be used
	 */
	void setAlgorithm(ArpeggioAlgorithm algorithm);

	/**
	 * Sets arpeggio sequence length (number of last added steps that will be played)
	 * @param size window size to be set
	 */
	void setWindowSize(unsigned char size);

	//Controllers

	/** resets arpeggiator to be played from beginning */
	void reset();

	/** clears all arpeggiator notes and resets it */
	void clear();

private:

	ArpeggioAlgorithm algorithm_;
	unsigned char windowSize_;
	unsigned char notes_[ARPEGGIATOR_MAX_VALUES];
	unsigned char sortedNotes_[ARPEGGIATOR_MAX_VALUES];
	unsigned char count_;
	bool started_;
	unsigned char currentIndex_;
	bool upDownGoingUp_;
	bool removeNoteAfterGettingNext_;

	bool findValue(unsigned char value, unsigned char * array, unsigned char & index);
	unsigned char getNextIndex();
	void fillSorted();
};

inline void Arpeggiator::setAlgorithm(Arpeggiator::ArpeggioAlgorithm algorithm) {
	algorithm_ = algorithm;
	upDownGoingUp_ = true;
}

inline void Arpeggiator::setWindowSize(unsigned char size) {
	windowSize_ = size;
}

inline void Arpeggiator::reset() {
	started_ = false;
}

inline void Arpeggiator::clear() {
	count_ = 0;
	reset();
}


#endif /* ARPEGGIATOR_H_ */
