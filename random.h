/*
 * random.h
 *
 *  Created on: 10.11.2014
 *      Author: user
 */

#ifndef RANDOM_H_
#define RANDOM_H_

//// Taken from Mozzi library to remove dependency on Arduino
//// Should be rethought and -written

unsigned long xorshift96();

void xorshiftSeed(unsigned long seed);


char rand(char minval, char maxval);
char rand(char maxval);

unsigned char rand(unsigned char minval, unsigned char maxval);
unsigned char rand(unsigned char maxval);

int rand(int minval, int maxval);
int rand(int maxval);

unsigned int rand(unsigned int minval, unsigned int maxval);
unsigned int rand(unsigned int maxval);

unsigned char randMidiNote();

#endif /* RANDOM_H_ */
