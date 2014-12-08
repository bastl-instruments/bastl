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

class StepRecorder {
public:
	StepRecorder();
	void init(Player * player, IStepMemory * memory, PlayerSettings * playerSettings);
	void record(bool record);
	void recordNote(unsigned char instrumentIndex);
	void recordMIDINote(unsigned char channel, unsigned char note);
private:
	Player * player_;
	IStepMemory * memory_;
	PlayerSettings * playerSettings_;

	bool record_;
};

inline void StepRecorder::record(bool record) {

	record_ = record;
}


#endif /* STEPRECORDER_H_ */
