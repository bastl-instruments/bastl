/*
 * portManipulations.h
 *
 *  Created on: 06.07.2014
 *      Author: user
 *      thanks to: http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&t=91304
 */

#ifndef PORTMANIPULATIONS_H_
#define PORTMANIPULATIONS_H_

// convenient macros for port manipulation
// usage:
//   - define pins like this (example: PB0)
//     	   #define NAME B,0
//     and us those defines as arguments for the following macros
//
//   - bit_set()			sets bit to HIGH
//     bit_clear()			sets bit to LOW
//     bit_read()   		returns the current value of the bit
//     bit_dir_outp()       sets data direction to output
//     bit_dir_inp()        sets data direction to input
//     bit_read_in()		performs quick digitalRead


#define   bit_set(...)       bit_set_(__VA_ARGS__)
#define   bit_set_(x,y)      PORT ## x |= 1<<y
#define   bit_clear(...)     bit_clear_(__VA_ARGS__)
#define   bit_clear_(x,y)    PORT ## x &= ~(1<<y)
#define   bit_read(...)      bit_read_(__VA_ARGS__)
#define   bit_read_(x,y)     (!!(PORT ## x & (1<<y)))

#define   bit_dir_outp(...)  bit_dir_outp_(__VA_ARGS__)
#define   bit_dir_outp_(x,y) DDR ## x |= 1<<y
#define   bit_dir_inp(...)   bit_dir_inp_(__VA_ARGS__)
#define   bit_dir_inp_(x,y)  DDR ## x &= ~(1<<y)
#define   bit_read_in(...)   bit_read_in_(__VA_ARGS__)
#define   bit_read_in_(x,y)  (!!(PIN ## x & (1<<y)))


// some lower-level version that use explicit register specification
// copied from: https://gist.github.com/holachek/3310645

//#define setOutput(ddr, pin) ((ddr) |= (1<< (pin)))
#define setLow(port, pin) ((port) &= ~(1 << (pin)))
#define setHigh(port, pin) ((port) |= (1 << (pin)))
//#define outputState(port, pin) ((port) & (1 << (pin)))
//#define pulse(port, pin) do { \
//setHigh((port), (pin)); \
//setLow((port), (pin)); \
//} while (0)




#endif /* PORTMANIPULATIONS_H_ */
