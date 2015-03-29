
#ifndef STEPPER_H_
#define STEPPER_H_

#include <inttypes.h>


class Stepper {
public:
	void init(uint16_t stepLength, uint8_t numbSteps) {
		setStepLength(stepLength);
		setNumbSteps(numbSteps);
	}

	void setStepLength(uint16_t stepLength) {
		this->stepLength = stepLength;
	}
	void setNumbSteps(uint8_t numbSteps) {
		this->numbSteps = numbSteps;
	}

	void setToStep(uint16_t time, uint8_t step) {
		lastTime = time;
		lastStep = step;
		timeFromLastStep = 0;
	}

	// increment internal counter up to last full step position
	uint8_t getCurrentStep(uint16_t time) {

		movePosition(time);

		return lastStep;
	}

	// increment to last full step position but also decide if next step is closer
	uint8_t getClosestStep(uint16_t time) {

		movePosition(time);

		uint8_t returnVal = lastStep;
		if (timeFromLastStep > stepLength/2) {
			returnVal++;
			if (returnVal == numbSteps) returnVal=0;
		}

		return returnVal;

	}

private:

	void movePosition(uint16_t time) {

		uint16_t timeDiff = timeFromLastStep + (time-lastTime);
		lastTime = time;

		while (timeDiff>=stepLength) {
			timeDiff -= stepLength;
			lastStep++;
			if (lastStep == numbSteps) lastStep = 0;
		}

		timeFromLastStep = timeDiff;
	}

	uint8_t numbSteps;
	uint16_t stepLength;

	uint16_t timeFromLastStep;
	uint16_t lastTime;
	uint8_t lastStep;

};



#endif /* STEPPER_H_ */
