/*
 * tst.cpp
 *
 *  Created on: Jul 16, 2014
 *      Author: Martin Baar
 */

#include "SteppingHWLayerMock.h"
#include "../Stepper.h"
#include "../MIDIStepper.h"
#include <iostream>

unsigned char numberOfSteps = 0;

void doStep() {
	numberOfSteps++;
}

int main( int argc, const char* argv[] ) {
	SteppingHWLayerMock hw;
	hw.setBastlCyclesPerSecond(128);
	hw.setElapsedBastlCycles(0);
	printf("Checking Stepper\n");
	Stepper stepper(&hw);
	stepper.setBPM(60);
	stepper.setStepCallback(&doStep);
	stepper.update();
	for (int i = 0; i < 8; i++) {
		hw.setElapsedBastlCycles(hw.getElapsedBastlCycles() + 1);
		stepper.update();
	}
	printf("Checking Stepper after 8 cycles: %s\n", numberOfSteps == 0 ? "OK": "ERROR");
	hw.setElapsedBastlCycles(hw.getElapsedBastlCycles() + 1);
	stepper.update();
	printf("Checking Stepper after another 1 cycle: %s\n", numberOfSteps == 1 ? "OK": "ERROR");
	for (int i = 0; i < 7; i++) {
		hw.setElapsedBastlCycles(hw.getElapsedBastlCycles() + 1);
		stepper.update();
	}
	printf("Checking Stepper after another 7 cycles: %s\n", numberOfSteps == 1 ? "OK": "ERROR");
	hw.setElapsedBastlCycles(hw.getElapsedBastlCycles() + 1);
	stepper.update();
	printf("Checking Stepper after another 1 cycle: %s\n", numberOfSteps == 2 ? "OK": "ERROR");

	printf("Testing changed BPM\n");

	stepper.setBPM(120);

	for (int i = 0; i < 3; i++) {
		hw.setElapsedBastlCycles(hw.getElapsedBastlCycles() + 1);
		stepper.update();
	}
	printf("Checking Stepper after another 15 cycles: %s\n", numberOfSteps == 2 ? "OK": "ERROR");
	hw.setElapsedBastlCycles(hw.getElapsedBastlCycles() + 1);
	stepper.update();
	printf("Checking Stepper after another 1 cycle: %s\n", numberOfSteps == 3 ? "OK": "ERROR");

	printf("Testing overflow\n");
	stepper.setBPM(60);
	hw.setElapsedBastlCycles((unsigned int)(-3));
	stepper.sync();
	numberOfSteps = 0;
	printf("Testing overflow init done.\n");
	for (int i = 0; i < 8; i++) {
		//std::cout << "Elapsed cycles " << hw.getElapsedBastlCycles() << std::endl;
		hw.setElapsedBastlCycles(hw.getElapsedBastlCycles() + 1);
		stepper.update();
	}
	printf("Checking Stepper after 8 cycles: %s\n", numberOfSteps == 0 ? "OK": "ERROR");
	hw.setElapsedBastlCycles(hw.getElapsedBastlCycles() + 1);
	stepper.update();
	printf("Checking Stepper after another 1 cycle: %s\n", numberOfSteps == 1 ? "OK": "ERROR");

	hw.setElapsedBastlCycles(0);
	numberOfSteps = 0;
	MIDIStepper midiStepper(&hw);
	midiStepper.setStepCallback(&doStep);
	midiStepper.stepMIDI();
	printf("Checking Stepper after another 1 MIDI step: %s\n", numberOfSteps == 1 ? "OK": "ERROR");
	for (int i = 0; i < 16; i++) {
		hw.setElapsedBastlCycles(hw.getElapsedBastlCycles() + 1);
		midiStepper.update();
	}
	printf("Checking Stepper after another 1 MIDI step and 16 Bastl cycles: %s\n", numberOfSteps == 1 ? "OK": "ERROR");
	midiStepper.stepMIDI();
	printf("Checking Stepper after another 2 MIDI step and 16 Bastl cycles: %s\n", numberOfSteps == 2 ? "OK": "ERROR");
	for (int i = 0; i < 8; i++) {
		hw.setElapsedBastlCycles(hw.getElapsedBastlCycles() + 1);
		midiStepper.update();
	}
	printf("Checking Stepper after another 2 MIDI step and 24 Bastl cycles: %s\n", numberOfSteps == 2 ? "OK": "ERROR");
	hw.setElapsedBastlCycles(hw.getElapsedBastlCycles() + 1);
	midiStepper.update();
	printf("Checking Stepper after another 2 MIDI step and 25 Bastl cycles: %s\n", numberOfSteps == 3 ? "OK": "ERROR");
	for (int i = 0; i < 7; i++) {
		hw.setElapsedBastlCycles(hw.getElapsedBastlCycles() + 1);
		midiStepper.update();
	}
	printf("Checking Stepper after another 2 MIDI step and 32 Bastl cycles: %s\n", numberOfSteps == 3 ? "OK": "ERROR");
	midiStepper.stepMIDI();
	printf("Checking Stepper after another 3 MIDI step and 32 Bastl cycles: %s\n", numberOfSteps == 4 ? "OK": "ERROR");


	for (int i = 0; i < 9; i++) {
		hw.setElapsedBastlCycles(hw.getElapsedBastlCycles() + 1);
		midiStepper.update();
	}
	printf("Checking Stepper after another 3 MIDI step and 41 Bastl cycles: %s\n", numberOfSteps == 5 ? "OK": "ERROR");
	for (int i = 0; i < 23; i++) {
		hw.setElapsedBastlCycles(hw.getElapsedBastlCycles() + 1);
		midiStepper.update();
	}
	printf("Checking Stepper after another 3 MIDI step and 64 Bastl cycles: %s\n", numberOfSteps == 5 ? "OK": "ERROR");
	midiStepper.stepMIDI();
	printf("Checking Stepper after another 4 MIDI step and 64 Bastl cycles: %s\n", numberOfSteps == 6 ? "OK": "ERROR");
	for (int i = 0; i < 17; i++) {
		hw.setElapsedBastlCycles(hw.getElapsedBastlCycles() + 1);
		midiStepper.update();
	}
	printf("Checking Stepper after another 4 MIDI step and 97 Bastl cycles: %s\n", numberOfSteps == 7 ? "OK": "ERROR");
}





