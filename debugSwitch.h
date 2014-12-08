/*
 * debugSwitch.h
 *
 *  Created on: 05.07.2014
 *      Author: user
 */

#ifndef DEBUGSWITCH_H_
#define DEBUGSWITCH_H_


// enclose your debug routines by this macro to quickyl turn them on or off
// 	DBGS 	debug start
// 	DBGE 	debug end

#define DEBUG 0

#define DBGS if(DEBUG) {
#define DBGE }




#endif /* DEBUGSWITCH_H_ */
