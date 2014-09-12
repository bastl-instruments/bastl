#include <iostream>
#include "../Tapper.h"
#include "../StepDivider.h"
#include "../StepMultiplier.h"
#include "../StepGenerator.h"

unsigned int lastStepTimeUnits = 0;
unsigned int numberOfTaps = 0;

unsigned int numberOfStepCallbacks = 0;

Tapper tapper;

void tapped() {
	numberOfTaps++;
	lastStepTimeUnits = tapper.getTimeUnitsPerStep();
}

void step() {
	numberOfStepCallbacks++;
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

	printf("Testing Step Multiplier\n");

	StepMultiplier multiplier;
	multiplier.init(100);
	multiplier.setStepCallback(&step);
	multiplier.setMultiplication(5);
	multiplier.setMinTriggerTime(1);

	multiplier.update(3000);
	ok = (numberOfStepCallbacks == 0 && multiplier.getTimeUnitsPerStep() == 50);
	printf("\tTest 1 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.update(3100);
	ok = (numberOfStepCallbacks == 0 && multiplier.getTimeUnitsPerStep() == 50);
	printf("\tTest 2 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.doStep(6100);
	ok = (numberOfStepCallbacks == 1 && multiplier.getTimeUnitsPerStep() == 50);
	printf("\tTest 3 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.update(6110);
	ok = (numberOfStepCallbacks == 1 && multiplier.getTimeUnitsPerStep() == 50);
	printf("\tTest 4 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.update(6111);
	ok = (numberOfStepCallbacks == 2 && multiplier.getTimeUnitsPerStep() == 50);
	printf("\tTest 5 - Update %s\n", ok ? "OK" : "Error");
		if (!ok) return 1;

	multiplier.update(6120);
	ok = (numberOfStepCallbacks == 2 && multiplier.getTimeUnitsPerStep() == 50);
	printf("\tTest 6 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.update(6121);
	ok = (numberOfStepCallbacks == 3 && multiplier.getTimeUnitsPerStep() == 50);
	printf("\tTest 7 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.doStep(6125);
	ok = (numberOfStepCallbacks == 4 && multiplier.getTimeUnitsPerStep() == 25);
	printf("\tTest 8 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.update(6126);
	ok = (numberOfStepCallbacks == 4 && multiplier.getTimeUnitsPerStep() == 25);
	printf("\tTest 9 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.update(6127);
	ok = (numberOfStepCallbacks == 5 && multiplier.getTimeUnitsPerStep() == 25);
	printf("\tTest 10 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.update(6128);
	ok = (numberOfStepCallbacks == 5 && multiplier.getTimeUnitsPerStep() == 25);
	printf("\tTest 11 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.update(6129);
	ok = (numberOfStepCallbacks == 6 && multiplier.getTimeUnitsPerStep() == 25);
	printf("\tTest 12 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.update(6130);
	ok = (numberOfStepCallbacks == 6 && multiplier.getTimeUnitsPerStep() == 25) ;
	printf("\tTest 13 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.update(6131);
	ok = (numberOfStepCallbacks == 7 && multiplier.getTimeUnitsPerStep() == 25);
	printf("\tTest 14 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.update(6135);
	ok = (numberOfStepCallbacks == 7 && multiplier.getTimeUnitsPerStep() == 25);
	printf("\tTest 15 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.update(6136);
	ok = (numberOfStepCallbacks == 8 && multiplier.getTimeUnitsPerStep() == 25);
	printf("\tTest 16 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.update(6140);
	ok = (numberOfStepCallbacks == 8 && multiplier.getTimeUnitsPerStep() == 25);
	printf("\tTest 16 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.update(6141);
	ok = (numberOfStepCallbacks == 9 && multiplier.getTimeUnitsPerStep() == 25);
	printf("\tTest 16 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.update(6145);
	ok = (numberOfStepCallbacks == 9 && multiplier.getTimeUnitsPerStep() == 25);
	printf("\tTest 16 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.update(6146);
	ok = (numberOfStepCallbacks == 10 && multiplier.getTimeUnitsPerStep() == 25);
	printf("\tTest 16 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.update(6150);
	ok = (numberOfStepCallbacks == 10 && multiplier.getTimeUnitsPerStep() == 25);
	printf("\tTest 16 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.update(6151);
	ok = (numberOfStepCallbacks == 10 && multiplier.getTimeUnitsPerStep() == 25);
	printf("\tTest 16 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.update(6175);
	ok = (numberOfStepCallbacks == 10 && multiplier.getTimeUnitsPerStep() == 25);
	printf("\tTest 16 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	multiplier.doStep(6175);
	ok = (numberOfStepCallbacks == 11 && multiplier.getTimeUnitsPerStep() == 50);
	printf("\tTest 16 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	printf("Testing Step Generator\n");

	StepGenerator generator;
	numberOfStepCallbacks = 0;
	generator.setStepCallback(&step);
	generator.setTimeUnitsPerStep(10);

	ok = (numberOfStepCallbacks == 0 && generator.getTimeUnitsPerStep() == 10);
	printf("\tTest 1 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	generator.update(1000);
	ok = (numberOfStepCallbacks == 1);
	printf("\tTest 2 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	generator.update(1010);
	ok = (numberOfStepCallbacks == 1);
	printf("\tTest 3 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	generator.update(1011);
	ok = (numberOfStepCallbacks == 2);
	printf("\tTest 4 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	generator.update(1022);
	ok = (numberOfStepCallbacks == 3);
	printf("\tTest 5 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	generator.update(1030);
	ok = (numberOfStepCallbacks == 3);
	printf("\tTest 6 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	generator.update(1031);
	ok = (numberOfStepCallbacks == 4);
	printf("\tTest 7 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	generator.doStep(1032);
	ok = (numberOfStepCallbacks == 4);
	printf("\tTest 8 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	generator.update(1045);
	ok = (numberOfStepCallbacks == 5);
	printf("\tTest 8 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	printf("Testing Step Divider\n");

	StepDivider divider;
	numberOfStepCallbacks = 0;
	divider.setStepCallback(&step);
	divider.setDivision(4);

	divider.update(100);
	divider.update(200);
	ok = (numberOfStepCallbacks == 0 && divider.getTimeUnitsPerStep() == 0);
	printf("\tTest 1 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	divider.doStep(300);
	ok = (numberOfStepCallbacks == 1 && divider.getTimeUnitsPerStep() == 0);
	printf("\tTest 2 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	divider.doStep(400);
	ok = (numberOfStepCallbacks == 1 && divider.getTimeUnitsPerStep() == 400);
	printf("\tTest 3 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	divider.doStep(500);
	ok = (numberOfStepCallbacks == 1 && divider.getTimeUnitsPerStep() == 400);
	printf("\tTest 4 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	divider.doStep(600);
	ok = (numberOfStepCallbacks == 1 && divider.getTimeUnitsPerStep() == 400);
	printf("\tTest 5 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	divider.doStep(700);
	ok = (numberOfStepCallbacks == 2 && divider.getTimeUnitsPerStep() == 400);
	printf("\tTest 6 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	divider.doStep(800);
	ok = (numberOfStepCallbacks == 2 && divider.getTimeUnitsPerStep() == 400);
	printf("\tTest 7 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	divider.doStep(900);
	ok = (numberOfStepCallbacks == 2 && divider.getTimeUnitsPerStep() == 400);
	printf("\tTest 8 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	divider.doStep(1000);
	ok = (numberOfStepCallbacks == 2 && divider.getTimeUnitsPerStep() == 400);
	printf("\tTest 9 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;

	divider.doStep(1100);
	ok = (numberOfStepCallbacks == 3 && divider.getTimeUnitsPerStep() == 400);
	printf("\tTest 10 - Update %s\n", ok ? "OK" : "Error");
	if (!ok) return 1;


	//multiplier.setMultiplication(5);
	//multiplier.setMinTriggerTime(1);
}
