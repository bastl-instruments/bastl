#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <inttypes.h>
#include "../LFOSimple.h"
#include "../LFORandom.h"


LFO * lfoPtr;

const uint16_t bastlCyclesPerSecond = 2000;

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


void render(const std::string& name, const std::string& title, uint32_t from, uint32_t to, uint8_t stepSize=1) {

	RedirStdOut::setToFile(name);
	printf("Timestamp %s\n",title.c_str());

	for (uint32_t timeStamp = from; timeStamp < to; timeStamp+=stepSize) {
		printf("%u ",timeStamp);
		printf("%u ",lfoPtr->getValue(timeStamp));
		printf("\n");
	}

	RedirStdOut::setToConsole();
}

void render(const std::string& name, uint16_t titleVal, uint32_t from, uint32_t to, uint8_t stepSize=1) {
	char title[10];
	sprintf(title,"%u",titleVal);
	render(name,title,from,to,stepSize);
}






int main( int argc, const char* argv[] ) {

	printf("### Size: %u Bytes\n",(uint16_t)sizeof(LFOSimple));
	printf("### Running with %u bastl cycle Frequency\n",bastlCyclesPerSecond);


	printf("# Frequencies\n");

	lfoPtr = new LFOSimple();
	lfoPtr->setFrequency(200);
	render("Freq1","200",0,400,1);
	delete lfoPtr;

	lfoPtr = new LFOSimple();
	lfoPtr->setFrequency(100);
	render("Freq2","100",0,400,1);
	delete lfoPtr;

	lfoPtr = new LFOSimple();
	lfoPtr->setFrequency(300);
	render("Freq3","300",0,400,1);
	delete lfoPtr;

	printf("# Waveforms\n");

	LFOSimple lfo;
	lfoPtr = &lfo;

	lfo.setFrequency(100);
	lfo.setWaveform(LFOSimple::SAW);
	render("Waveform1","Saw",0,500,1);


	lfo.setFrequency(100);
	lfo.setWaveform(LFOSimple::TRIANGLE);
	render("Waveform2","Triangle",0,500,1);


	LFORandom rand;
	lfoPtr = &rand;
	rand.setFrequency(100);
	render("Waveform3","Random",0,500,1);


	printf("# Resolution\n");
	for (uint8_t index=0; index<8; index++) {

		const uint8_t map[8] = {0,200,220,240,250,253,254,255};

		uint8_t val = map[index];
		lfo.setNumbStepsToSkip(val);
		lfoPtr = &lfo;
		char name[15];
		sprintf(name,"Res%u",index);
		render(name,val,index*100,(index+1)*100,1);
	}


	printf("# Smoothness\n");
	for (uint8_t index=0; index<6; index++) {
		lfoPtr = &rand;
		rand.setFrequency(100);
		rand.setSmoothness(50*index);
		char name[15];
		sprintf(name,"Smooth%u",50*index);
		render(name,50*index,0,1000,1);
	}


	printf("# Smoothness Low\n");
	for (uint8_t index=0; index<6; index++) {
		rand.setFrequency(100);
		rand.setSmoothness(5*index);
		char name[15];
		sprintf(name,"Smooth%u",5*index);
		render(name,5*index,0,1000,1);
	}


	printf("# Flopping\n");
	for (uint8_t index=0; index<8; index++) {
		uint8_t val = 1<<index;
		lfo.setFlop(val);
		lfo.setNumbStepsToSkip(0);
		lfoPtr = &lfo;
		char name[15];
		sprintf(name,"Flop%u",index);
		render(name,val,index*100,(index+1)*100,1);
	}

	printf("# Flopping Rand\n");
	for (uint8_t index=0; index<8; index++) {
		uint16_t val = 1<<index;
		rand.setFlop(val);
		rand.setSmoothness(0);
		lfoPtr = &rand;
		char name[15];
		sprintf(name,"FlopRand%u",index);
		render(name,val,index*100,(index+1)*100,1);
	}


	printf("# XOR\n");
	for (uint8_t index=0; index<8; index++) {
		uint16_t val = 1<<index;
		lfo.setFlop(0);
		lfo.setXOR(val);
		lfoPtr = &lfo;
		char name[15];
		sprintf(name,"Xor%u",index);
		render(name,val,index*100,(index+1)*100,1);
	}

	printf("# XOR Rand\n");
	for (uint8_t index=0; index<8; index++) {
		uint16_t val = 1<<index;
		rand.setFlop(0);
		rand.setXOR(val);
		rand.setSmoothness(100);
		lfoPtr = &rand;
		char name[15];
		sprintf(name,"XorRand%u",index);
		render(name,val,index*100,(index+1)*100,1);
	}

	printf("# Folding \n");
	for (uint8_t index=0; index<8; index++) {
		lfo.setFlop(0);
		lfo.setXOR(0);
		uint16_t val = 255-index*30;
		lfo.setFolding(val);
		lfoPtr = &lfo;
		char name[15];
		sprintf(name,"Fold%u",index);
		render(name,val,index*100,(index+1)*100,1);
	}

	printf("# Folding Rand\n");
	for (uint8_t index=0; index<8; index++) {
		rand.setFrequency(20);
		rand.setFlop(0);
		rand.setXOR(0);
		rand.setSmoothness(100);
		uint16_t val = 255-index*30;
		rand.setFolding(val);
		lfoPtr = &rand;
		char name[15];
		sprintf(name,"FoldRand%u",index);
		render(name,val,index*100,(index+1)*100,1);
	}


	printf("# Overflow \n");
	for (uint8_t index=0; index<8; index++) {
		lfo.setFlop(0);
		lfo.setXOR(0);
		uint16_t val = 255-index*30;
		lfo.setOverflow(val);
		lfoPtr = &lfo;
		char name[15];
		sprintf(name,"Overflow%u",index);
		render(name,val,index*100,(index+1)*100,1);
	}

	printf("# Overflow Rand\n");
	for (uint8_t index=0; index<8; index++) {
		rand.setFrequency(20);
		rand.setFlop(0);
		rand.setXOR(0);
		rand.setSmoothness(100);
		uint16_t val = 255-index*30;
		rand.setOverflow(val);
		lfoPtr = &rand;
		char name[15];
		sprintf(name,"OverflowRand%u",index);
		render(name,val,index*100,(index+1)*100,1);
	}



/*

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

	render(1,5000,LFOs,4,15);



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
		LFOs[index].setWaveform(SAW);

		uint8_t FlopBit = 1<<6;
		LFOs[index].setFlop(FlopBit);
		printf("%u ",FlopBit);
	}
	printf("\n");

	render(1,2000,LFOs,4,1);




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
*/

}
