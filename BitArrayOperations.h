/*
 * BitArrayOperations.h
 *
 *  Created on: Sep 3, 2014
 *      Author: bastl
 */

#ifndef BITARRAYOPERATIONS_H_
#define BITARRAYOPERATIONS_H_


#define GETBIT(X, Y)  (((X) & (1 << (Y))) >> (Y) == 1)
#define SETBITTRUE(X, Y) ((X) = ((X) | (1 << (Y))))
#define SETBITFALSE(X, Y) ((X) = ((X) & ~(1 << (Y))))
#define SETBIT(X, Y, Z) (Z ? ((X) = ((X) | (1 << (Y)))) : ((X) = ((X) & ~(1 << (Y)))))


#endif /* BITARRAYOPERATIONS_H_ */
