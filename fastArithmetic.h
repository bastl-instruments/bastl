/*
 * fastArithmetic.h
 *
 *  Created on: 05.08.2014
 *      Author: user
 */

#ifndef FASTARITHMETIC_H_
#define FASTARITHMETIC_H_

static inline int16_t S16U8MulShift8(int16_t a, uint8_t b) {
  int16_t result;
  asm(
    "eor %B0, %B0"    "\n\t"
    "mul %A1, %A2"    "\n\t"
    "mov %A0, r1"     "\n\t"
    "mulsu %B1, %A2"  "\n\t"
    "add %A0, r0"     "\n\t"
    "adc %B0, r1"     "\n\t"
    "eor r1, r1"      "\n\t"
    : "=&r" (result)
    : "a" (a), "a" (b)
  );
  return result;
}

static inline uint8_t U8U8MulShift8(uint8_t a, uint8_t b) {
  uint8_t result;
  asm(
    "mul %1, %2" "\n\t"
    "mov %0, r1" "\n\t"
    "eor r1, r1" "\n\t"
    : "=r" (result)
    : "a" (a), "a" (b)
  );
  return result;
}
static inline uint16_t U16U8MulShift8(uint16_t a, uint8_t b) {
  uint16_t result;
  asm(
    "eor %B0, %B0" "\n\t"
    "mul %A1, %A2" "\n\t"
    "mov %A0, r1" "\n\t"
    "mul %B1, %A2" "\n\t"
    "add %A0, r0" "\n\t"
    "adc %B0, r1" "\n\t"
    "eor r1, r1" "\n\t"
    : "=&r" (result)
    : "a" (a), "a" (b)
  );
  return result;
}

static inline int16_t S16S8MulShift8(int16_t a, int8_t b) {
  int16_t result;
  asm(
    "eor %B0, %B0"    "\n\t"
    "muls %A2, %B1"   "\n\t"
    "movw %A0, r0"    "\n\t"
    "mulsu %A2, %A1"  "\n\t"
    "eor r0, r0"      "\n\t"
    "sbc %B0, r0"     "\n\t"
    "add %A0, r1"     "\n\t"
    "adc %B0, r0"     "\n\t"
    "eor r1, r1"      "\n\t"
    : "=&r" (result)
    : "a" (a), "a" (b)
  );
  return result;
}

static inline uint8_t S16ClipU8(int16_t value) {
  uint8_t result;
  asm(
    "mov %0, %A1"   "\n\t"  // by default, copy the value.
    "or %B1, %B1"   "\n\t"  // load H to set flags.
    "brpl .+4"      "\n\t"  // if positive, skip
    "ldi %0, 0"     "\n\t"  // set to 0.
    "rjmp .+4"      "\n\t"  // and jump.
    "breq .+2"      "\n\t"  // if null, skip
    "ldi %0, 255"   "\n\t"  // set to 255
    : "=r" (result)
    : "a" (value)
  );
  return result;
}

static inline int16_t Clip(int16_t value, int16_t min, int16_t max) {
  return value < min ? min : (value > max ? max : value);
}



#endif /* FASTARITHMETIC_H_ */
