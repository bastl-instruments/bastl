/*
 * fixedMath.h
 *
 *  Created on: 10.11.2014
 *      Author: user
 */

#ifndef FIXEDMATH_H_
#define FIXEDMATH_H_

////// Taken from Mozzi library to remove explicit dependency
////// Will be restructured in the future


typedef char Q0n7; 				/**< signed fractional number using 7 fractional bits, represents -0.5 to 0.496*/
typedef char Q7n0;				/**< ordinary old signed Q7n0 char with 0 fractional bits, represents -128 to 127*/
typedef unsigned char Q0n8;  		/**< unsigned fractional number using 8 fractional bits, represents 0.0 to 0.996*/
typedef unsigned char Q8n0;  		/**< normal unsigned char with 0 fractional bits, represents 0.0 to 255.0*/
typedef unsigned int Q0n16;  		/**< unsigned fractional number using 16 fractional bits, represents 0.0 to 0.999*/
typedef unsigned long Q0n31;				/**< signed number using 0 integer bits and 31 fractional bits, represents -32768 to 32767*/
typedef int Q7n8; 				/**< signed fractional number using 7 integer bits and 8 fractional bits, represents -127.996 to 127.996*/
typedef unsigned int Q3n13; /**< unsigned fractional number using 3 integer bits and 13 fractional bits, represents 0 to 7.999*/
typedef int Q1n14;				/**< signed fractional number using 1 integer bit and 14 fractional bits, represents -1.999 to 1.999*/
typedef int Q15n0;				/**< signed number using 15 integer bits and 0 fractional bits, represents -2147483648 to 2147483647*/
typedef unsigned int Q8n8;		/**< unsigned fractional number using 8 integer bits and 8 fractional bits, represents 0 to 255.996*/
typedef unsigned int	 Q1n15;	/**< unsigned fractional number using 1 integer bit and 15 fractional bits, represents 0 to 1.999*/
typedef unsigned int	 Q16n0;	/**< unsigned number using 16 integer bits and 0 fractional bits, represents 0 to 65536.0*/
typedef long Q23n8;				/**< signed fractional number using 23 integer bits and 8 fractional bits, represents -8388607.996 to 8388607.996*/
typedef long Q15n16;			/**< signed fractional number using 15 integer bits and 16 fractional bits, represents -32767.999 to 32767.999*/
typedef long Q31n0;				/**< signed (normal long int) number using 31 integer bits and 0 fractional bits, represents -2147483648 to 2147483647*/
typedef unsigned long Q32n0;		/**< unsigned (normal unsigned long int) number using 32 integer bits and 0 fractional bits, represents 0 to 4294967295*/
typedef unsigned long	Q0n32;		/**< unsigned (normal unsigned long int) number using 0 integer bits and 32 fractional bits, represents 0 to 0.999999999767169*/
typedef unsigned long	Q8n24;		/**< signed fractional number using 8 integer bits and 24 fractional bits, represents 0 to 255.999*/
typedef unsigned long Q24n8;		/**< unsigned fractional number using 24 integer bits and 8 fractional bits, represents 0 to  16777215*/
typedef unsigned long Q16n16;	/**< unsigned fractional number using 16 integer bits and 16 fractional bits, represents 0 to 65535.999*/



#endif /* FIXEDMATH_H_ */
