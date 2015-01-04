/*
 * BitArrayOperations.h
 *
 *  Created on: Sep 3, 2014
 *      Author: bastl
 */

#ifndef BITARRAYOPERATIONS_H_
#define BITARRAYOPERATIONS_H_


#define GETBIT(X, Y)  (((X) & (1 << (Y))) != 0)
#define SETBITTRUE(X, Y) ((X) = ((X) | (1 << (Y))))
#define SETBITFALSE(X, Y) ((X) = ((X) & ~(1 << (Y))))
#define SETBIT(X, Y, Z) (Z ? ((X) = ((X) | (1 << (Y)))) : ((X) = ((X) & ~(1 << (Y)))))

class BitArrayOperations {

public:
	static void setBit(unsigned char & bitArray, unsigned char bitIndex, bool value) {
		if (value) {
			bitArray = bitArray | (1 << bitIndex);
		} else {
			bitArray = bitArray & ~(1 << bitIndex);
		}
	}

	static bool getBit(unsigned char bitArray, unsigned char bitIndex) {
		return (bitArray & (1 << bitIndex)) != 0;
	}
};

#endif /* BITARRAYOPERATIONS_H_ */
