
#ifndef STEPPER_H_
#define STEPPER_H_

#include <inttypes.h>
#define minimum(a,b) ((a)<(b)?(a):(b))

// This object quantizes time in uints of stepLength and returns the step number modulo numbsteps
// 	((time (- offset)) / stepLength) % numbSteps
// as time is overflowing, this object needs to work on time differences, which creates the nescessity
// 		for it to keep a memory of the current step time
// This has the disadvantage (over 'direct formula') that you cannot jump back in time but the advantage that
// 		tempo changes are being handled correctly with no effort
// The maximum inaccuracy defines how many time units before a step a call to getClosestStep() may map to that step



class Stepper {
public:

	// initialize an instance, setting its essential properties
	void init(uint16_t stepLength, uint8_t numbSteps, uint8_t maxInaccuracy = 255) {
		setStepLength(stepLength);
		setNumbSteps(numbSteps);
		this-> maxInaccuracy = maxInaccuracy;
	}

	// change the time difference between two steps
	void setStepLength(uint16_t stepLength) {
		if (stepLength == 0) return;

		this->stepLength = stepLength;
	}

	// set the number of steps after which the step number will wrap to zero
	void setNumbSteps(uint8_t numbSteps) {
		if (numbSteps == 0) return;

		this->numbSteps = numbSteps;

		// map current position to changed period length
		while (lastStep >= numbSteps) {
			lastStep -= numbSteps;
		}
	}

	// set the playhead to a given step number at a given time
	void setToStep(uint16_t time, uint8_t step) {
		lastTime = time;
		lastStep = step;
		timeFromLastStep = 0;
	}

	// proceed to the given point in time and return the last step number
	uint8_t getCurrentStep(uint16_t time) {

		movePosition(time);
		return lastStep;
	}

	// proceed to the given point in time
	// return the step number that is closest (can be last or previous one)
	uint8_t getClosestStep(uint16_t time) {

		movePosition(time);

		uint8_t returnVal = lastStep;
		if ((stepLength-timeFromLastStep) < minimum(stepLength/2,maxInaccuracy)) {
			incStep(returnVal);
		}

		return returnVal;

	}

private:

	// proceed to the given point in time
	void movePosition(uint16_t time) {

		uint16_t timeDiff = timeFromLastStep + (time-lastTime);
		lastTime = time;

		while (timeDiff>=stepLength) {
			timeDiff -= stepLength;
			incStep(lastStep);
		}

		timeFromLastStep = timeDiff;
	}

	// increment step number with wrapping
	inline void incStep(uint8_t& val) {
		val++;
		if (val >= numbSteps) val=0;
	}

	uint8_t maxInaccuracy;		// the maximum number of time units a call to getClosestStep() maps forward in time

	uint8_t numbSteps; 			// the number of steps after which the step number wraps to zero
	uint16_t stepLength;		// the number of time units between two steps

	uint16_t timeFromLastStep;	// the number of time units the current position is ahead of a step
	uint16_t lastTime;			// the last time this instance was called
	uint8_t lastStep;			// the number of the last complete step

};



#endif /* STEPPER_H_ */
