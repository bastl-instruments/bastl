#ifndef PLAYER_H
#define PLAYER_H

//#define DEBUG

#include "IStepMemory.h"
#include "RackInstrumentDefinitions.h"
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
    Player(IStepMemory * memory,
    	   PlayerSettings * settings,
    	   StepSynchronizer * synchronizer,
    	   void (*instrumentEventCallback)(unsigned char instrumentID,
    			   	   	   	   	   	   	   DrumStep::DrumVelocityType velocityType,
    			   	   	   	   	   	   	   bool isOn)
    	   );
    void stepFourth();
    unsigned char getCurrentInstrumentStep(unsigned char instrumentID);
    void setCurrentInstrumentStep(unsigned char instrumentID, unsigned char step);
    void changeActivesForCurrentStep(unsigned char instrumentID, unsigned char numberOfActiveSteps);
    void changeActivesForCurrentStepInAllInstrunents(unsigned char numberOfActiveSteps);

    unsigned char getCurrentInstrumentSubStep(unsigned char instrumentID);
    void playNote(unsigned char instrumentID, DrumStep::DrumVelocityType velocityType);
    void resetAllInstruments();
    void startLoop(unsigned char step);
    void stopLoop();
    bool isPlaying();
    void update(unsigned int elapsedTimeUnits);
private:
    IStepMemory * memory_;
    PlayerSettings * settings_;
    StepSynchronizer * synchronizer_;
    unsigned char currentSteps_[INSTRUMENTS];
    unsigned char playingInstruments[ALL_INSTRUMENTS_IN_BYTES];
    bool isStopped_;
    bool inLoop_;
    unsigned char loopedStep_;
    void (*instrumentEventCallback_)(unsigned char instrumentID, DrumStep::DrumVelocityType velocityType, bool isOn);
    unsigned int lastElapsedTimeUnits_;
    unsigned int lastDummyPlayInstrumentTimeUnits_;
    void stepDrumInstruments();
    bool isInstrumentPlaying(unsigned char instrumentID);
    void setInstrumentPlaying(unsigned char instrumentID, bool isPlaying);
    void sendNoteOffIfPlaying(unsigned char instrumentID);

};

inline void Player::startLoop(unsigned char step) {
	inLoop_ = true;
	loopedStep_ = step;
}

inline void Player::stopLoop() {
	inLoop_ = false;
}

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

inline bool Player::isPlaying() {
	return !isStopped_;
}
#endif // PLAYER_H
