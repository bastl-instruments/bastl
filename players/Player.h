#ifndef PLAYER_H
#define PLAYER_H

//#define DEBUG

#include "../data/IStepMemory.h"
#include "../data/MIDICommand.h"
#include "../data/InstrumentDefinitions.h"
#include "IMIDICommandProcessor.h"
#include "PlayerSettings.h"

#ifdef DEBUG

#include <iostream>

#endif

/**
 * @brief The Player class that plays defined pattern for soecific instruments
 */
class Player
{
public:
    Player(IStepMemory * memory, IMIDICommandProcessor* midiProcessor, PlayerSettings * settings);
    void stepFourth();
private:
    IStepMemory * memory_;
    IMIDICommandProcessor * midiProcessor_;
    PlayerSettings * settings_;
    unsigned char currentSteps_[INSTRUMENTS];
    unsigned char playingInstruments[ALL_INSTRUMENTS_IN_BYTES];
    unsigned char currentPatterns_[1];

    void stepDrumInstruments();
    bool isInstrumentPlaying(unsigned char instrumentID);
    void setInstrumentPlaying(unsigned char instrumentID, bool isPlaying);

};

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
