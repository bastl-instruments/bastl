/*
 * basic.h
 *
 *  Created on: 04.02.2015
 *      Author: user
 */

#ifndef BASIC_H_
#define BASIC_H_
#ifndef Arduino_h // this is not compatible with Arduino.h because macro names are identical
#define Arduino_h

// some basic stuff that used to reside in Arduino.h
// this has been moved to a separate file to remove dependency on Arduino.h where possible


#include <inttypes.h>


#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))

#endif /* Arduino_h */
#endif /* BASIC_H_ */
