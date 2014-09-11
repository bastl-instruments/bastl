#include <iostream>
#include "../Tapper.h"

unsigned int lastStepTimeUnits = 0;
unsigned int numberOfTaps = 0;
Tapper tapper;

void tapped() {
	numberOfTaps++;
	lastStepTimeUnits = tapper.getTimeUnitsPerStep();
}

int main( int argc, const char* argv[] ) {

	printf("Testing Tapper");
	tapper.init(100, 10);
	tapper.setStepCallBack(&tapped);


	//Very basic test
	tapper.tap(10);
	bool ok = (numberOfTaps == 1 && lastStepTimeUnits == 0);
	printf("\tTest 1 - Tap %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	tapper.tap(20);
	ok = (numberOfTaps == 2 && lastStepTimeUnits == 10);
	printf("\tTest 2 - Tap %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	tapper.tap(30);
	ok = (numberOfTaps == 3 && lastStepTimeUnits == 10);
	printf("\tTest 3 - Tap %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	//Max Step length test

	tapper.tap(140);
	ok = (numberOfTaps == 4 && lastStepTimeUnits == 0);
	printf("\tTest 4 - Tap %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	//testing averages from steps

	tapper.tap(160);
	ok = (numberOfTaps == 5 && lastStepTimeUnits == 20);
	printf("\tTest 5 - Tap %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	tapper.tap(178);
	ok = (numberOfTaps == 6 && lastStepTimeUnits == 19);
	printf("\tTest 6 - Tap %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	tapper.tap(194);
	ok = (numberOfTaps == 7 && lastStepTimeUnits == 18);
	printf("\tTest 7 - Tap %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	//testing max deviation from average

	tapper.tap(204);
	ok = (numberOfTaps == 8 && lastStepTimeUnits == 16);
	printf("\tTest 8 - Tap %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	tapper.tap(211);
	ok = (numberOfTaps == 9 && lastStepTimeUnits == 0);
	printf("\tTest 9 - Tap %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

}
