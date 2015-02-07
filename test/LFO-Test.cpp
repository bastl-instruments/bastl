#include <stdio.h>
#include <inttypes.h>
#include "../lfoExtended.h"

const uint8_t numbObjects = 4;

float frequencies[numbObjects] = {0.1,1,3.2,4};
uint16_t bastlCyclesPerSecond = 3000;

lfoExtended LFOs[numbObjects];





int main( int argc, const char* argv[] ) {

	//printf("## Running with %u bast cycle Frequency",bastlCyclesPerSecond);

	printf("Timestamp ");
	for (uint8_t index=0; index<numbObjects; index++) {
		LFOs[index].init();
		uint16_t bastlCyclesPerPeriod = (float)bastlCyclesPerSecond/(frequencies[index]);
		printf("%u ",bastlCyclesPerPeriod);
		LFOs[index].setBastlCyclesPerPeriod(bastlCyclesPerPeriod);
		LFOs[index].setWaveform(TRIANGLE);
		//LFOs[index].setToStep(0,150);
		LFOs[index].setResolution(0);
		LFOs[index].setThreshold(255);
	}
	printf("\n");

	;

	for (uint32_t timeStamp = 64000; timeStamp < 69000; timeStamp+=1) {

		printf("%u ",timeStamp);

		for (uint8_t index=0; index<numbObjects; index++) {
			printf("%u ",LFOs[index].getValue(timeStamp));
		}

		printf("\n");
	}


}
