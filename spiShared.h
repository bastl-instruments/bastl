/*
 * spiShared.h
 *
 *  Created on: 05.08.2014
 *      Author: user
 */

#ifndef SPISHARED_H_
#define SPISHARED_H_



#include <Arduino.h>
#include "portManipulations.h"

// SPI Pins
#define SCK B,5
#define MISO B,4
#define MOSI B,3
#define SS B,2


static inline __attribute__((always_inline)) byte spiRead() {
  SPDR = 0xFF; // start SPI clock
  while (!(SPSR & _BV(SPIF)));
  return SPDR;
}

static inline __attribute__((always_inline)) byte  spiWrite(byte data) {
  SPDR = data;
  while (!(SPSR & _BV(SPIF)));
  return SPDR;
}


#endif /* SPISHARED_H_ */
