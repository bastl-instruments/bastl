#include <stdio.h>
#include <inttypes.h>
#include "../lfoExtended.h"

const uint8_t numbObjects = 4;

float frequencies[numbObjects] = {0.1,1,3,5};
uint16_t bastlCyclesPerSecond = 300;

lfoExtended LFOs[numbObjects];





int main( int argc, const char* argv[] ) {

	for (uint8_t index=0; index<numbObjects; index++) {
		LFOs[index].init();
		uint16_t bastlCyclesPerPeriod = (float)bastlCyclesPerSecond/(frequencies[index]);
		LFOs[index].setBastlCyclesPerPeriod(bastlCyclesPerPeriod);
		LFOs[index].setWaveform(RANDOM,false,false);
		LFOs[index].setToStep(0,40);
		LFOs[index].setResolution(0);
		LFOs[index].setThreshold(255);
	}


	uint32_t timeStamp = 0;

	for (uint16_t index=0; index < 100; index++) {
		timeStamp += 1;

		printf("%u ",timeStamp);

		for (uint8_t index=0; index<numbObjects; index++) {
			printf("%u ",LFOs[index].getValue(timeStamp));
		}

		printf("\n");
	}


}
