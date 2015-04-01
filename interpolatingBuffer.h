
#ifndef INTERPOLATING_BUFFER_H_
#define INTERPOLATING_BUFFER_H_

#include <random.h>
<<<<<<< HEAD

//#include "basic.h"
=======
#include <Arduino.h>

>>>>>>> lfoInterrupt
class interpolatingBuffer {

public:
	void init(uint8_t channel);
	void setBastlCyclesPerPeriod(uint16_t _steps){periodInSteps=_steps;};
	void loopRandom(bool _loopRandom){ loopState=_loopRandom;};
	void interpolateMode(bool _interpolateMode){ interpolate=_interpolateMode;};
	void setInterpolationAmount(uint8_t _interpolationAmount){ interpolateAmount=_interpolationAmount; };
	void setSmootingAmount(uint16_t _smoothingAmount){ smoothingAmount=_smoothingAmount; };

	void update();
	void setStepValue(uint8_t _step,uint8_t _value){ buffer[_step]=_value; };
	void setStep(uint8_t _step);
	void addRandomStep();
	void addSmoothStep(uint8_t _value);
	void sync();
	void setCurrentStep(uint8_t _step){ step=_step;};
	uint8_t getCurrentValue();


private:
	bool loopState;
	uint8_t intCh;
	bool interpolate;
	uint8_t buffer[32];
	uint8_t smoothBuffer[32];
	uint8_t smoothingStep;
	uint8_t step;
	uint8_t interpolateAmount;
	uint16_t smoothingAmount;
	uint16_t periodInSteps;
	uint16_t smallSteps;
	uint16_t smoothSteps;
	uint16_t interpolatedSteps;
	uint16_t interpolateInSteps;
	uint8_t interpolateStep;
	void rotateBuffer();
	void rotateSmoothBuffer();
	void incrementPhase();
	uint8_t calculateSmoothAverage();
	//void calcCurrentValue();

};





#endif
