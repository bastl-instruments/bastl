#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <inttypes.h>
#include "../lfoExtended.h"

const uint8_t numbObjects = 4;

float frequencies[numbObjects] = {0.1,1,3.2,4};
uint16_t bastlCyclesPerSecond = 4000;

lfoExtended LFOs[numbObjects];

class RedirStdOut {
public:

	static void setToFile(std::string filename) {

	static const std::string subfolder = "data";

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

FILE* RedirStdOut::fp;






int main( int argc, const char* argv[] ) {

	printf("### Size: %u Bytes\n",(uint16_t)sizeof(lfoExtended));
	printf("### Running with %u bastl cycle Frequency\n",bastlCyclesPerSecond);

	// Timestamp overflow region
	printf("# Checking region of timestamp overflow\n");
	RedirStdOut::setToFile("lfoTimeOverflow.csv");

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
	RedirStdOut::setToConsole();
	printf("# Different waveforms\n");
	RedirStdOut::setToFile("lfoWaveforms.csv");


	for (uint8_t index=0; index<numbObjects; index++) {
		LFOs[index].init();
		LFOs[index].setBastlCyclesPerPeriod((float)bastlCyclesPerSecond/3);
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
	RedirStdOut::setToConsole();
	printf("# Resolution\n");
	RedirStdOut::setToFile("lfoResolution.csv");

	printf("Timestamp ");
	for (uint8_t index=0; index<numbObjects; index++) {
		LFOs[index].init();
		LFOs[index].setBastlCyclesPerPeriod((float)bastlCyclesPerSecond/3);

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



	// XOR
	RedirStdOut::setToConsole();
	printf("# XOR\n");
	RedirStdOut::setToFile("lfoXOR.csv");

	printf("Timestamp ");
	for (uint8_t index=0; index<numbObjects; index++) {
		LFOs[index].init();
		LFOs[index].setBastlCyclesPerPeriod((float)bastlCyclesPerSecond/3);

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



	// XOR
	RedirStdOut::setToConsole();
	printf("# Flop\n");
	RedirStdOut::setToFile("lfoFlop.csv");

	printf("Timestamp ");
	for (uint8_t index=0; index<numbObjects; index++) {
		LFOs[index].init();
		LFOs[index].setBastlCyclesPerPeriod((float)bastlCyclesPerSecond/3);
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


	// Threshold
	RedirStdOut::setToConsole();
	printf("# Threshold\n");
	RedirStdOut::setToFile("lfoThres.csv");


	for (uint8_t index=0; index<numbObjects; index++) {
		LFOs[index].init();
		LFOs[index].setBastlCyclesPerPeriod((float)bastlCyclesPerSecond/3);
		LFOs[index].setWaveform(TRIANGLE);
	}

	printf("Timestamp ");
	uint8_t thres;
	thres = 200;
	LFOs[0].setThreshold(thres);
	printf("%u ",thres);
	thres = 60;
	LFOs[1].setThreshold(thres);
	printf("%u ",thres);
	thres = 200;
	LFOs[2].setThreshold(thres,OVERFLOW);
	printf("%u ",thres);
	thres = 60;
	LFOs[3].setThreshold(thres,OVERFLOW);
	printf("%u ",thres);

	printf("\n");

	for (uint32_t timeStamp = 1; timeStamp < 4000; timeStamp+=1) {
		printf("%u ",timeStamp);
		for (uint8_t index=0; index<numbObjects; index++) {
			printf("%u ",LFOs[index].getValue(timeStamp));
		}
		printf("\n");
	}

	// Internal parameters
	RedirStdOut::setToConsole();
	printf("# Internal Parameters\n");
	RedirStdOut::setToFile("lfoInternal.csv");

	LFOs[0].init();
	LFOs[0].setBastlCyclesPerPeriod(1250);

	printf("Timestamp ");
	printf("TimestampU16 ");
	printf("LastTimestamp ");
	printf("TimestampOffset ");
	printf("CurrentStep ");
	printf("Output");
	printf("\n");

	for (uint32_t timeStamp = 64000; timeStamp < 69000; timeStamp+=1) {
		printf("%u ",timeStamp);
		uint8_t val = LFOs[0].getValue(timeStamp);
		printf("%u\n",val);
	}


}
