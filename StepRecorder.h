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
	void update();
	void record(bool record);
	unsigned int startRecordNote(unsigned char instrumentIndex);
	void stopRecordNote(unsigned char instrumentIndex);
	void recordMIDINote(unsigned char channel, unsigned char note);
	void setCurrentStepper(BastlStepper * stepper);
private:
	Player * player_;
	IStepMemory * memory_;
	PlayerSettings * playerSettings_;
	BastlStepper * stepper_;

	bool record_;
	bool recordInstrumentStatuses_[6];

	unsigned char getCurrentQuantizedSubstep(unsigned char instrumentIndex);
	void recordSubstep(unsigned char instrumentIndex, unsigned char subStepIndex);


};

inline void StepRecorder::record(bool record) {

	record_ = record;
}

inline void StepRecorder::setCurrentStepper(BastlStepper * stepper) {
	stepper_ = stepper;
}


#endif /* STEPRECORDER_H_ */
