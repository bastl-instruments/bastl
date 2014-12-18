/*
 * StepRecorder.h
 *
 *  Created on: Aug 28, 2014
 *      Author: bastl
 */

#ifndef STEPRECORDER_H_
#define STEPRECORDER_H_

#include "Player.h"
#include "PlayerSettings.h"
#include "IStepMemory.h"
#include <BastlStepper.h>

class StepRecorder {
public:
	StepRecorder();
	void init(Player * player, IStepMemory * memory, PlayerSettings * playerSettings, BastlStepper * stepper);
	void record(bool record);
	void recordNote(unsigned char instrumentIndex);
	void recordMIDINote(unsigned char channel, unsigned char note);
	void setCurrentStepper(BastlStepper * stepper);
private:
	Player * player_;
	IStepMemory * memory_;
	PlayerSettings * playerSettings_;
	BastlStepper * stepper_;

	bool record_;
};

inline void StepRecorder::record(bool record) {

	record_ = record;
}

inline void StepRecorder::setCurrentStepper(BastlStepper * stepper) {
	stepper_ = stepper;
}


#endif /* STEPRECORDER_H_ */
