/*
 * movingAverage.h
 *
 *  Created on: 13.11.2014
 *      Author: user
 */

#ifndef MOVINGAVERAGE_H_
#define MOVINGAVERAGE_H_

// A linear Moving Average Class
//
// the size of the buffer is passed in the constructor
// datatype is set by a template parameter
//
// Values are weightened by index*(255-(255/size))
//
// works only for uint8_t and uint16_t!

template <typename type>
class MovingAverageLinear {
public:

	// construct an object with the given buffer size
	MovingAverageLinear(uint8_t size);

	~MovingAverageLinear();

	// add another value to the buffer
	void add(type value);

	// calculate current average of all elements in buffer
	type getAverage();

	// delete all elements from buffer but set a value that is returned as average
	void clear();

	// returns the number of valid elements in the buffer
	uint8_t getFillCount() { return fillCount;}

	// individual access to buffer elements
	type operator[](uint8_t index) {return values[index];}

private:
	bool needUpdate;
	type average;
	type* values;
	uint8_t size;
	uint8_t fillCount;
	uint8_t linearDecrease;
};


template <typename type>
MovingAverageLinear<type>::MovingAverageLinear(uint8_t size) {
	values = new type[size];
	this->size = size;
	clear();
	linearDecrease = 255/(size);
}

template <typename type>
MovingAverageLinear<type>::~MovingAverageLinear() {
	if (values != 0) delete values;
}

template <typename type>
void MovingAverageLinear<type>::clear() {
	fillCount = 0;
}

template <typename type>
void MovingAverageLinear<type>::add(type value) {

	// shift all one back
	for (uint8_t index=fillCount; index>=1; index--) {
		values[index] = values[index-1];
	}

	// add current value at front
	values[0] = value;

	// update status
	if (fillCount<size) fillCount++;
	needUpdate = true;
}

template <typename type>
type MovingAverageLinear<type>::getAverage() {

	// calculate new average if needed
	if (needUpdate && fillCount) {
		uint32_t tmpVal = 0;
		uint16_t tmpWeight = 0;
		for (uint8_t index=0; index<fillCount; index++) {
			tmpVal += values[index]*(255-(index*linearDecrease));
			tmpWeight += (255-(index*linearDecrease));
		}
		average = tmpVal/tmpWeight;
		needUpdate = false;
	}

	// return current average
	return average;

}

#endif /* MOVINGAVERAGE_H_ */
