#include "Player.h"
#include "IStepMemory.h"

#define SUBSTEPS_PER_STEP 4

Player::Player(IStepMemory * memory,
			   PlayerSettings * settings,
			   StepSynchronizer * synchronizer,
			   void (*instrumentEventCallback)(unsigned char instrumentID, DrumStep::DrumVelocityType velocityType, bool isOn)
			   ) : memory_(memory),
			   	   settings_(settings),
			   	   synchronizer_(synchronizer),
			   	   isStopped_(true),
			   	   inLoop_(false),
			   	   loopedStep_(6),
			   	   instrumentEventCallback_(instrumentEventCallback)
{
    for (unsigned char i = 0; i < INSTRUMENTS; i++) {
        currentSteps_[i] = 0;
    }
    for (unsigned char i = 0 ; i < ALL_INSTRUMENTS_IN_BYTES; i++) {
        playingInstruments[i] = 0;
    }
}

void Player::stepFourth()
{
    stepDrumInstruments();
}

void Player::playNote(unsigned char instrumentID, DrumStep::DrumVelocityType velocityType) {
	sendNoteOffIfPlaying(instrumentID);
	if (instrumentEventCallback_) {
		instrumentEventCallback_(instrumentID, velocityType, true);
	}
	setInstrumentPlaying(instrumentID, true);
}

void Player::sendNoteOffIfPlaying(unsigned char instrumentID) {
	if (isInstrumentPlaying(instrumentID)) {
		if (instrumentEventCallback_) {
			instrumentEventCallback_(instrumentID, DrumStep::OFF , false);
		}
		setInstrumentPlaying(instrumentID, false);
	}
}

void Player::stepDrumInstruments()
{
    for (unsigned char i = 0; i < 6; i++) {
        DrumStep nextStep;
        unsigned char nextSubStepIndex = (currentSteps_[i] + 1) % 256;
        if (isStopped_ ) {
        	nextSubStepIndex = currentSteps_[i];
        }
        bool nextStepExists = true;
        // when in the substep sequence dont ask for the next step otherwise ask for newxt available
        if (nextSubStepIndex % 4 != 0) {
            unsigned char nextStepIndex = nextSubStepIndex / 4;
            //printf("calling currentStep on for %i index is %i \n", i, nextStepIndex);
            nextStep = memory_->getDrumStep(i, nextStepIndex);
        } else {
        	if (inLoop_) {
        		nextStep = memory_->getDrumStep(i, loopedStep_);
        		nextSubStepIndex = 4 * loopedStep_;
        	} else {
        		unsigned char currentStepIndex = nextSubStepIndex / 4;
        		//printf("calling nextStep on for %i index is %i \n", i, currentStepIndex);
        		nextStepExists = memory_->getNextActiveDrumStep(i, currentStepIndex, nextStep);
        		//if (nextStepExists) printf("NextStepExists index %i\n", currentStepIndex);
        		nextSubStepIndex = 4 * currentStepIndex;
        	}
        }
    	sendNoteOffIfPlaying(i);
        if (nextStepExists) {
            if (settings_->isInstrumentOn(i) && !nextStep.isMuted() && nextStep.isActive()) {
                DrumStep::DrumVelocityType type = nextStep.getSubStep(nextSubStepIndex % 4);
                if (type != DrumStep::OFF) {
                	playNote(i, type) ;
                }

            }
            currentSteps_[i] = nextSubStepIndex;
        }
        //printf("\n");
    }
    isStopped_ = false;
}

void Player::changeActivesForCurrentStep(unsigned char instrumentID, unsigned char numberOfActiveSteps) {
	setCurrentInstrumentStep(instrumentID, (synchronizer_->getCurrentStepNumber() / 4) % numberOfActiveSteps);
}

void Player::changeActivesForCurrentStepInAllInstrunents(unsigned char numberOfActiveSteps) {
	for (unsigned char instrument = 0; instrument < 6; instrument++) {
		changeActivesForCurrentStep(instrument, numberOfActiveSteps);
	}
}

void Player::resetAllInstruments() {
	for (unsigned char i = 0; i < 6; i++) {
		sendNoteOffIfPlaying(i);
		currentSteps_[i] = 0;
	}
	isStopped_ = true;
}
