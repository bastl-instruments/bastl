#ifndef INSTRUMENTDEFINITIONS_H
#define INSTRUMENTDEFINITIONS_H


#define STEPS_PER_PATTERN 64

#define DRUM_INSTRUMENTS 6
#define DRUM_PATTERNS 1
#define DRUM_STEPS DRUM_INSTRUMENTS * DRUM_PATTERNS * STEPS_PER_PATTERN
#define DRUM_BITS_PER_STEP 6
#define DRUM_BYTES (DRUM_STEPS * DRUM_BITS_PER_STEP) / 8

#define MONO_INSTRUMENTS 0
#define MONO_PATTERNS 0
#define MONO_STEPS MONO_INSTRUMENTS * MONO_PATTERNS * STEPS_PER_PATTERN

#define INSTRUMENTS DRUM_INSTRUMENTS + MONO_INSTRUMENTS

#define ALL_INSTRUMENTS_IN_BYTES 3

#endif // INSTRUMENTDEFINITIONS_H
