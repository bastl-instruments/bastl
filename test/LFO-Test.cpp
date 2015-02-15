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


void render(uint32_t from, uint32_t to, lfoExtended* objects, uint8_t numbObjects) {
	for (uint32_t timeStamp = from; timeStamp < to; timeStamp++) {

		printf("%.2f ",(float)1000*timeStamp/bastlCyclesPerSecond);

		for (uint8_t index=0; index<numbObjects; index++) {
			objects[index].step();
			printf("%u ",objects[index].getValue());
		}

		printf("\n");
	}
}






int main( int argc, const char* argv[] ) {

	printf("### Size: %u Bytes\n",(uint16_t)sizeof(lfoExtended));
	printf("### Running with %u bastl cycle Frequency\n",bastlCyclesPerSecond);




	printf("# Checking region of timestamp overflow\n");
	RedirStdOut::setToFile("lfoFrequencies.csv");
	printf("Timestamp ");
	for (uint8_t index=0; index<numbObjects; index++) {
		LFOs[index].init(bastlCyclesPerSecond);
		uint16_t bastlCyclesPerPeriod = (float)bastlCyclesPerSecond/(frequencies[index]);
		printf("%.2fHz ",frequencies[index]);
		LFOs[index].setBastlCyclesPerPeriod(bastlCyclesPerPeriod);
		LFOs[index].setWaveform(SAW);
	}
	printf("\n");

	render(0,10000,LFOs,4);
	RedirStdOut::setToConsole();



	printf("# Different waveforms\n");
	RedirStdOut::setToFile("lfoWaveforms.csv");
	for (uint8_t index=0; index<numbObjects; index++) {
		LFOs[index].init(bastlCyclesPerSecond);
		uint16_t bastlCyclesPerPeriod = (float)bastlCyclesPerSecond/(1);
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

	render(1,10000,LFOs,4);
	RedirStdOut::setToConsole();




	// Resolution
	RedirStdOut::setToConsole();
	printf("# Resolution\n");
	RedirStdOut::setToFile("lfoResolution.csv");

	printf("Timestamp ");
	for (uint8_t index=0; index<numbObjects; index++) {
		LFOs[index].init(bastlCyclesPerSecond);
		LFOs[index].setBastlCyclesPerPeriod((float)bastlCyclesPerSecond/3);

		uint8_t res = 2*(index)+1;
		LFOs[index].setResolution(res);
		LFOs[index].setWaveform(SAW);
		printf("%u ",res);
	}
	printf("\n");

	render(1,5000,LFOs,4);



	// XOR
	RedirStdOut::setToConsole();
	printf("# XOR\n");
	RedirStdOut::setToFile("lfoXOR.csv");

	printf("Timestamp ");
	for (uint8_t index=0; index<numbObjects; index++) {
		LFOs[index].init(bastlCyclesPerSecond);
		LFOs[index].setBastlCyclesPerPeriod((float)bastlCyclesPerSecond/3);

		uint8_t XorBits = (1<<(index+2))+index;
		LFOs[index].setXOR(XorBits);
		printf("%u ",XorBits);
	}
	printf("\n");

	render(1,5000,LFOs,4);



	// Flop
	RedirStdOut::setToConsole();
	printf("# Flop\n");
	RedirStdOut::setToFile("lfoFlop.csv");

	printf("Timestamp ");
	for (uint8_t index=0; index<numbObjects; index++) {
		LFOs[index].init(bastlCyclesPerSecond);
		LFOs[index].setBastlCyclesPerPeriod((float)bastlCyclesPerSecond/3);
		LFOs[index].setWaveform(TRIANGLE);

		uint8_t FlopBit = 4;
		LFOs[index].setFlop(1<<FlopBit);
		printf("%u ",FlopBit);
	}
	printf("\n");

	render(1,7000,LFOs,4);




	// Threshold
	RedirStdOut::setToConsole();
	printf("# Threshold\n");
	RedirStdOut::setToFile("lfoThres.csv");


	for (uint8_t index=0; index<numbObjects; index++) {
		LFOs[index].init(bastlCyclesPerSecond);
		LFOs[index].setBastlCyclesPerPeriod(bastlCyclesPerSecond/3);
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

	render(1,6000,LFOs,4);



	// Set to step
	RedirStdOut::setToConsole();
	printf("# Set to step\n");
	RedirStdOut::setToFile("lfoSetStep.csv");


	for (uint8_t index=0; index<numbObjects; index++) {
		LFOs[index].init(bastlCyclesPerSecond);
		LFOs[index].setBastlCyclesPerPeriod(bastlCyclesPerSecond/3);
	}


	printf("Timestamp ");
	uint16_t resetTimes [numbObjects] = {1000,1200,3619,4895};
	for (uint8_t index=0; index<numbObjects; index++) {
		printf("%u ",resetTimes[index]);
	}
	printf("\n");

	render(1,resetTimes[0],LFOs,4);
	LFOs[0].setToStep(0);
	render(resetTimes[0],resetTimes[1],LFOs,4);
	LFOs[1].setToStep(0);
	render(resetTimes[1],resetTimes[2],LFOs,4);
	LFOs[2].setToStep(0);
	render(resetTimes[2],resetTimes[3],LFOs,4);
	LFOs[3].setToStep(3);
	render(resetTimes[3],resetTimes[3]+1000,LFOs,4);





	// RANDOM Waveform
	RedirStdOut::setToConsole();
	printf("# Random Waveform\n");
	RedirStdOut::setToFile("lfoRandom.csv");


	for (uint8_t index=0; index<numbObjects; index++) {
		LFOs[index].init(bastlCyclesPerSecond);
		LFOs[index].setWaveform(RANDOM);
	}

	printf("Timestamp ");
	uint16_t period;

	period = 20;
	LFOs[0].setBastlCyclesPerPeriod(period);
	printf("%u ",period);

	period = 300;
	LFOs[0].setBastlCyclesPerPeriod(period);
	printf("%u ",period);

	period = 1000;
	LFOs[0].setBastlCyclesPerPeriod(period);
	printf("%u ",period);

	period = 3000;
	LFOs[0].setBastlCyclesPerPeriod(period);
	printf("%u ",period);

	printf("\n");

	render(0,5000,LFOs,4);





#ifdef INTERNAL
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
#endif


}
