/*
 * random.cpp
 *
 *  Created on: 10.11.2014
 *      Author: user
 */


#include "random.h"

#define min(a,b) ((a)<(b)?(a):(b))


uint8_t bastlRandom::byte() {
	static uint16_t rand_seed = 42;

	rand_seed ^= rand_seed << 7;
	rand_seed ^= rand_seed >> 11;
	rand_seed ^= rand_seed << 2;

	return rand_seed >> 8;
}


// works only for max-min <= 254
// not equally distribiuted!
uint8_t bastlRandom::range(uint8_t min, uint8_t max) {
	uint16_t numbChoices = max-min+1;
	return ((min(byte(),254)*numbChoices)>>8) + min;
}
