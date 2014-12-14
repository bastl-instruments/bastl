#ifndef PLAYER_H
#define PLAYER_H

//#define DEBUG

#include "IStepMemory.h"
#include "MIDICommand.h"
#include "RackInstrumentDefinitions.h"
#include "IMIDICommandProcessor.h"
#include "PlayerSettings.h"
#include "StepSynchronizer.h"


#ifdef DEBUG

#include <iostream>

#endif

/**
 * @brief The Player class that plays defined pattern for soecific instruments
 */
class Player
{
public:
    Player(IStepMemory * memory, IMIDICommandProcessor* midiProcessor, PlayerSettings * settings, StepSynchronizer * synchronizer);
    void stepFourth();
    unsigned char getCurrentInstrumentStep(unsigned char instrumentID);
    void setCurrentInstrumentStep(unsigned char instrumentID, unsigned char step);
    void changeActivesForCurrentStep(unsigned char instrumentID, unsigned char numberOfActiveSteps);
    unsigned char getCurrentInstrumentSubStep(unsigned char instrumentID);
    void playNote(unsigned char instrumentID, DrumStep::DrumVelocityType velocityType);

private:
    IStepMemory * memory_;
    IMIDICommandProcessor * midiProcessor_;
    PlayerSettings * settings_;
    StepSynchronizer * synchronizer_;
    unsigned char currentSteps_[INSTRUMENTS];
    unsigned char playingInstruments[ALL_INSTRUMENTS_IN_BYTES];
    bool isStopped_;

    void stepDrumInstruments();
    bool isInstrumentPlaying(unsigned char instrumentID);
    void setInstrumentPlaying(unsigned char instrumentID, bool isPlaying);
    void sendNoteOffIfPlaying(unsigned char instrumentID);

};

inline unsigned char Player::getCurrentInstrumentStep(unsigned char instrumentID) {
	return currentSteps_[instrumentID] / 4;
}

inline void Player::setCurrentInstrumentStep(unsigned char instrumentID, unsigned char step) {
	currentSteps_[instrumentID] = step * 4 + currentSteps_[instrumentID] % 4;
}

inline unsigned char Player::getCurrentInstrumentSubStep(unsigned char instrumentID) {
	return currentSteps_[instrumentID] % 4;
}


inline bool Player::isInstrumentPlaying(unsigned char instrumentID)
{
#ifdef DEBUG
    printf("Instrument %i, DataIndex %i,Data  %i\n", instrumentID, instrumentID / 8, playingInstruments[instrumentID / 8]);
    printf("dataAfterSwitch : %i \n", (playingInstruments[instrumentID / 8] & (1 << (instrumentID % 8))));
#endif
    return (((playingInstruments[instrumentID / 8] & (1 << (instrumentID % 8))) >> (instrumentID % 8))) == 1;
}

inline void Player::setInstrumentPlaying(unsigned char instrumentID, bool isPlaying)
{
    if (isPlaying) {
#ifdef DEBUG
        printf("Set playing true Instrument %i, DataIndex %i,Data  %i\n", instrumentID, instrumentID / 8, playingInstruments[instrumentID / 8]);
#endif
        playingInstruments[instrumentID / 8] = playingInstruments[instrumentID / 8] | (1 << (instrumentID % 8));
    } else {
#ifdef DEBUG
        printf("Set playing false Instrument %i, DataIndex %i,Data  %i\n", instrumentID, instrumentID / 8, playingInstruments[instrumentID / 8]);
#endif
        playingInstruments[instrumentID / 8] = playingInstruments[instrumentID / 8] & ~(1 << (instrumentID % 8));
    }
}

#endif // PLAYER_H
