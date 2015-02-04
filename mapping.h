/*
 * mapping.h
 *
 *  Created on: 10.11.2014
 *      Author: user
 */

#ifndef MAPPING_H_
#define MAPPING_H_

#include "basic.h"

#ifndef TESTING
#include <avr/pgmspace.h>


const uint16_t expByteToWord_Table[256] PROGMEM =
{
    0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3,
    3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 7,
    7, 7, 8, 8, 8, 9, 9, 10, 10, 10, 11, 11, 12, 12, 13, 13, 14, 15,
    15, 16, 17, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    31, 32, 33, 35, 36, 38, 40, 41, 43, 45, 47, 49, 52, 54, 56, 59,
    61, 64, 67, 70, 73, 76, 79, 83, 87, 91, 95, 99, 103, 108, 112,
    117, 123, 128, 134, 140, 146, 152, 159, 166, 173, 181, 189, 197,
    206, 215, 225, 235, 245, 256, 267, 279, 292, 304, 318, 332, 347,
    362, 378, 395, 412, 431, 450, 470, 490, 512, 535, 558, 583, 609,
    636, 664, 693, 724, 756, 790, 825, 861, 899, 939, 981, 1024, 1069,
    1117, 1166, 1218, 1272, 1328, 1387, 1448, 1512, 1579, 1649, 1722,
    1798, 1878, 1961, 2048, 2139, 2233, 2332, 2435, 2543, 2656, 2773,
    2896, 3025, 3158, 3298, 3444, 3597, 3756, 3922, 4096, 4277, 4467,
    4664, 4871, 5087, 5312, 5547, 5793, 6049, 6317, 6596, 6889, 7194,
    7512, 7845, 8192, 8555, 8933, 9329, 9742, 10173, 10624, 11094,
    11585, 12098, 12634, 13193, 13777, 14387, 15024, 15689, 16384,
    17109, 17867, 18658, 19484, 20346, 21247, 22188, 23170, 24196,
    25267, 26386, 27554, 28774, 30048, 31378, 32768, 34218, 35733,
    37315, 38967, 40693, 42494, 44376, 46340, 48392, 50534, 52772,
    55108, 57548, 60096, 62757, 65535
};

const uint16_t exp8BitTo12BitTable[256] PROGMEM =
{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,
		2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,
		7,8,8,8,9,9,9,10,10,10,11,11,12,12,12,13,13,14,14,15,15,16,17,17,
		18,18,19,20,20,21,22,23,23,24,25,26,27,28,29,30,31,32,33,34,35,37,
		38,39,40,42,43,45,46,48,49,51,53,55,57,58,60,63,65,67,69,71,74,76,
		79,81,84,87,90,93,96,99,103,106,110,113,117,121,125,129,134,138,143,
		147,152,157,163,168,174,180,185,192,198,205,211,218,226,233,241,249,
		257,266,275,284,293,303,313,323,334,345,357,368,381,393,406,420,434,
		448,463,478,494,511,527,545,563,582,601,621,641,662,684,707,730,755,
		780,805,832,860,888,917,948,979,1011,1045,1079,1115,1152,1190,1230,
		1270,1312,1356,1400,1447,1494,1544,1595,1648,1702,1758,1816,1877,1939,
		2003,2069,2137,2208,2281,2356,2434,2514,2598,2683,2772,2864,2958,3056,
		3157,3261,3369,3480,3595,3714,3837,3964,4095
};

inline uint8_t mapProgmemU8U8(uint8_t input, const uint8_t table[]) {
	return pgm_read_byte_near(table + input);
}

inline uint16_t mapProgmemU8U16(uint8_t input, const uint16_t table[]) {
	return pgm_read_word_near(table + input);
}
#endif


// maps the range of a input byte to a given range of an unsigned int
// this is fast but NOT fully reaching the end value!
inline uint16_t mapLinearRuntimeFastU8U16(uint8_t input, uint16_t start, uint16_t end) {
	return (((int32_t)input*(end-start))>>8)+start;
}

// maps the range of a input byte to a given range of an unsigned int
// this is slower but reaching the end value
inline uint16_t mapLinearRuntimePreciseU8U16(uint8_t input, uint16_t start, uint16_t end) {
	return (((int32_t)input*(end-start))/255)+start;
}

// maps the range of a input byte to a given range of an signed int
// this is fast but NOT fully reaching the end value!
inline int16_t mapLinearRuntimeFastU8S16(uint8_t input, int16_t start, int16_t end) {
	return (((int32_t)input*(end-start))>>8)+start;
}

// maps the range of a input byte to a given range of an signed int
// this is slower but reaching the end value
inline int16_t mapLinearRuntimePreciseU8S16(uint8_t input, int16_t start, int16_t end) {
	return (((int32_t)input*(end-start))/255)+start;
}



#endif /* MAPPING_H_ */
