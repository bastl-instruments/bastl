#include "Player.h"
#include "../data/IStepMemory.h"
#include "IMIDICommandProcessor.h"
#include "../data/MIDICommand.h"

#define SUBSTEPS_PER_STEP 4

Player::Player(IStepMemory * memory, IMIDICommandProcessor * midiProcessor, PlayerSettings * settings) : memory_(memory),
                                                                                                      midiProcessor_(midiProcessor),
                                                                                                      settings_(settings),
                                                                                                      isStopped_(true)

{
    for (unsigned char i = 0; i < INSTRUMENTS; i++) {
        currentSteps_[i] = 0;
    }
    for (unsigned char i = 0 ; i < ALL_INSTRUMENTS_IN_BYTES; i++) {
        playingInstruments[i] = 0;
    }
    currentPatterns_[0] = 0;
}

void Player::stepFourth()
{
    stepDrumInstruments();
}

void Player::stepDrumInstruments()
{
    for (unsigned char i = 0; i < DRUM_INSTRUMENTS; i++) {
        DrumStep nextStep;
        unsigned char nextSubStepIndex = (currentSteps_[i] + 1) % (SUBSTEPS_PER_STEP * STEPS_PER_PATTERN);
        if (isStopped_ ) {
        	isStopped_ = false;
        	nextSubStepIndex = currentSteps_[i];
        }
        bool nextStepExists = true;
        // when in the substep sequence dont ask for the next step otherwise ask for newxt available
        if (nextSubStepIndex % SUBSTEPS_PER_STEP != 0) {
            unsigned char nextStepIndex = nextSubStepIndex / SUBSTEPS_PER_STEP;
            //printf("calling currentStep on for %i index is %i \n", i, nextStepIndex);
            nextStep = memory_->getDrumStep(i, currentPatterns_[0], nextStepIndex);
        } else {
            unsigned char currentStepIndex = nextSubStepIndex / SUBSTEPS_PER_STEP;
            //printf("calling nextStep on for %i index is %i \n", i, currentStepIndex);
            nextStepExists = memory_->getNextActiveDrumStep(i, currentPatterns_[0], currentStepIndex, nextStep);
            //if (nextStepExists) printf("NextStepExists index %i\n", currentStepIndex);
            nextSubStepIndex = SUBSTEPS_PER_STEP * currentStepIndex;
        }

        if (nextStepExists) {
            if (settings_->isInstrumentOn(InstrumentTypes::DRUM, i)) {
                //printf("ON ");
                if (isInstrumentPlaying(i)) {
                    //printf("Playing ");
                    MIDICommand command(MIDICommand::NOTEOFF,
                    					settings_->getInstrumentChannel(InstrumentTypes::DRUM , i),
                    		            settings_->getDrumInstrumentNote(i),
                    					0);
                    midiProcessor_->SendCommand(command);
                    setInstrumentPlaying(i, false);
                }

                if (!nextStep.isMuted()) {
                    //printf("Not muted");
                    DrumStep::DrumVelocityType type = nextStep.getSubStep(nextSubStepIndex % SUBSTEPS_PER_STEP);
                    if (type != DrumStep::OFF) {

                        unsigned char velocity = settings_->getMIDIVelocityFromDrumVelocity(nextStep.getSubStep(type));
                        MIDICommand command(MIDICommand::NOTEON,
                        					settings_->getInstrumentChannel(InstrumentTypes::DRUM , i),
                        					settings_->getDrumInstrumentNote(i),
                        					velocity) ;
                        midiProcessor_->SendCommand(command);
                        setInstrumentPlaying(i, true);
                    }
                }
            }
            currentSteps_[i] = nextSubStepIndex;
        }
        //printf("\n");
    }
}
