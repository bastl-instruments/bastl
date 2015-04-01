#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <inttypes.h>
#include "../Stepper.h"




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



Stepper step;



int main( int argc, const char* argv[] ) {

	printf("Starting\n");

	RedirStdOut::setToFile("Stepper.csv");

	uint16_t time = 0;
	uint8_t inc = 1;
	step.init(15,32,3);
	step.setToStep(time,0);

	printf("Timestamp currentStep closestStep\n");

	for (uint32_t realTime=time; realTime<1000; realTime+=inc,time+=inc) {
		printf("%u %u %u \n",realTime,step.getCurrentStep(time), step.getClosestStep(time));
		if (realTime == 300) {
			step.setToStep(time,0);
		}
	}

	printf("done\n");



}
