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

//namespace BitArrayOperations {

void setBit(unsigned int & bitArray, unsigned char bitIndex, bool value);
void setBit(unsigned char & bitArray, unsigned char bitIndex, bool value);
bool getBit(unsigned char bitArray, unsigned char bitIndex);
//}

#endif /* BITARRAYOPERATIONS_H_ */
