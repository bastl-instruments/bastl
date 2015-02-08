#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <inttypes.h>
#include "../lfoExtended.h"

const uint8_t numbObjects = 4;

float frequencies[numbObjects] = {0.1,1,3.2,4};
uint16_t bastlCyclesPerSecond = 3000;

lfoExtended LFOs[numbObjects];

class RedirStdOut {
public:

	static void setToFile(std::string filename) {

		const std::string subfolder = "data";

		filename = subfolder + "/" + filename;

		if((fp=freopen(filename.c_str(), "w" ,stdout))==NULL) {
			printf("Cannot open file.\n");
			exit(1);
		}
	}

	static void setToConsole() {
		fclose(fp);
		freopen ("/dev/tty", "a", stdout);
	}

private:
	static FILE *fp;


};







int main( int argc, const char* argv[] ) {

	printf("### Running with %u bastl cycle Frequency\n",bastlCyclesPerSecond);

	// Timestamp overflow region
	printf("# Checking region of timestamp overflow\n");
	setStdOut("lfoTimeOverflow.csv");

	printf("Timestamp ");
	for (uint8_t index=0; index<numbObjects; index++) {
		LFOs[index].init();
		uint16_t bastlCyclesPerPeriod = (float)bastlCyclesPerSecond/(frequencies[index]);
		printf("%u ",bastlCyclesPerPeriod);
		LFOs[index].setBastlCyclesPerPeriod(bastlCyclesPerPeriod);
		LFOs[index].setWaveform(TRIANGLE);
	}
	printf("\n");


	for (uint32_t timeStamp = 64000; timeStamp < 69000; timeStamp+=1) {
		printf("%u ",timeStamp);
		for (uint8_t index=0; index<numbObjects; index++) {
			printf("%u ",LFOs[index].getValue(timeStamp));
		}
		printf("\n");
	}

	// Basic Waveforms
	resetStdOut();
	printf("# Different waveforms\n");
	setStdOut("lfoWaveforms.csv");


	for (uint8_t index=0; index<numbObjects; index++) {
		LFOs[index].init();
		uint16_t bastlCyclesPerPeriod = (float)bastlCyclesPerSecond/3;
		LFOs[index].setBastlCyclesPerPeriod(bastlCyclesPerPeriod);
	}
	printf("Timestamp ");
	LFOs[0].setWaveform(SAW);
	printf("SAW ");
	LFOs[1].setWaveform(TRIANGLE);
	printf("TRIANGLE ");
	LFOs[2].setWaveform(RANDOM);
	printf("RANDOM ");
	printf("\n");

	for (uint32_t timeStamp = 1; timeStamp < 4000; timeStamp+=1) {
		printf("%u ",timeStamp);
		for (uint8_t index=0; index<numbObjects; index++) {
			printf("%u ",LFOs[index].getValue(timeStamp));
		}
		printf("\n");
	}


	// Basic Waveforms
	resetStdOut();
	printf("# Resolution\n");
	setStdOut("lfoResolution.csv");

	printf("Timestamp ");
	for (uint8_t index=0; index<numbObjects; index++) {
		LFOs[index].init();
		uint16_t bastlCyclesPerPeriod = (float)bastlCyclesPerSecond/3;
		LFOs[index].setBastlCyclesPerPeriod(bastlCyclesPerPeriod);

		uint8_t res = 1<<(2*(index)+3);
		LFOs[index].setResolution(res);
		LFOs[index].setWaveform(SAW);
		printf("%u ",res);
	}
	printf("\n");

	for (uint32_t timeStamp = 1; timeStamp < 4000; timeStamp+=1) {
		printf("%u ",timeStamp);
		for (uint8_t index=0; index<numbObjects; index++) {
			printf("%u ",LFOs[index].getValue(timeStamp));
		}
		printf("\n");
	}

	resetStdOut();

	// XOR
	resetStdOut();
	printf("# XOR\n");
	setStdOut("lfoXOR.csv");

	printf("Timestamp ");
	for (uint8_t index=0; index<numbObjects; index++) {
		LFOs[index].init();
		uint16_t bastlCyclesPerPeriod = (float)bastlCyclesPerSecond/3;
		LFOs[index].setBastlCyclesPerPeriod(bastlCyclesPerPeriod);

		uint8_t XorBits = (1<<(index+2))+index;
		LFOs[index].setXOR(XorBits);
		printf("%u ",XorBits);
	}
	printf("\n");

	for (uint32_t timeStamp = 1; timeStamp < 4000; timeStamp+=1) {
		printf("%u ",timeStamp);
		for (uint8_t index=0; index<numbObjects; index++) {
			printf("%u ",LFOs[index].getValue(timeStamp));
		}
		printf("\n");
	}

	resetStdOut();

	// XOR
	resetStdOut();
	printf("# Flop\n");
	setStdOut("lfoFlop.csv");

	printf("Timestamp ");
	for (uint8_t index=0; index<numbObjects; index++) {
		LFOs[index].init();
		uint16_t bastlCyclesPerPeriod = (float)bastlCyclesPerSecond/3;
		LFOs[index].setBastlCyclesPerPeriod(bastlCyclesPerPeriod);
		LFOs[index].setWaveform(TRIANGLE);

		uint8_t FlopBit = 4;
		LFOs[index].setFlop(1<<FlopBit);
		printf("%u ",FlopBit);
	}
	printf("\n");

	for (uint32_t timeStamp = 1; timeStamp < 4000; timeStamp+=1) {
		printf("%u ",timeStamp);
		for (uint8_t index=0; index<numbObjects; index++) {
			printf("%u ",LFOs[index].getValue(timeStamp));
		}
		printf("\n");
	}

	resetStdOut();

}
