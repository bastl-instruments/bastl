/*
 * movingAverage.h
 *
 *  Created on: 13.11.2014
 *      Author: user
 */

#ifndef MOVINGAVERAGE_H_
#define MOVINGAVERAGE_H_

template <typename type>
class MovingAverageLinear {
public:
	MovingAverageLinear(uint8_t size);
	~MovingAverageLinear();

	void add(type value);
	type getAverage();
	void clear(uint8_t value = 0);

	uint8_t getFillCount() { return fillCount;}
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
void MovingAverageLinear<type>::clear(uint8_t value) {
	average = value;
	fillCount = 0;
	needUpdate = true;
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
