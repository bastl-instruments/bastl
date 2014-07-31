#ifndef PLAYERSETTINGS_H
#define PLAYERSETTINGS_H

#include "../data/InstrumentDefinitions.h"
#include "../data/DrumStep.h"

class PlayerSettings
{
public:
    PlayerSettings();

    unsigned char getDrumInstrumentNote(unsigned char instrumentID);
    void setDrumInstrumentNote(unsigned char instrumentID, unsigned char note);

    unsigned char getInstrumentChannel(InstrumentTypes::InstrumentType type, unsigned char instrumentID);
    void setInstrumentChannel(InstrumentTypes::InstrumentType type, unsigned char instrumentID, unsigned char channel);

    unsigned char getMIDIVelocityFromDrumVelocity(DrumStep::DrumVelocityType type);
    void setMIDIVelocitiesForDrumVelocities(unsigned char upDrumVelocity, unsigned char downDrumVelocity, unsigned char normalDrumVelocity);

    bool isInstrumentOn(InstrumentTypes::InstrumentType type, unsigned char instrumentID);
    void setInstrumentOn(InstrumentTypes::InstrumentType type, unsigned char instrumentID, bool isOn);

    void setCurrentPattern(unsigned char pattern);
    unsigned char getCurrentPattern();

private:
    unsigned char drumInstrumentNotes_[DRUM_INSTRUMENTS];
    unsigned char instrumentChannels_[(DRUM_INSTRUMENTS + MONO_INSTRUMENTS) / 2];
    unsigned char instrumentStatuses_[ALL_INSTRUMENTS_IN_BYTES];
    unsigned char upDrumVelocity_;
    unsigned char downDrumVelocity_;
    unsigned char normalDrumVelocity_;
    unsigned char currentPattern_;
};

inline unsigned char PlayerSettings::getDrumInstrumentNote(unsigned char instrumentID)
{
    return drumInstrumentNotes_[instrumentID];
}

inline void PlayerSettings::setDrumInstrumentNote(unsigned char instrumentID, unsigned char note)
{
    drumInstrumentNotes_[instrumentID] = note;
}

inline void PlayerSettings::setMIDIVelocitiesForDrumVelocities(unsigned char upDrumVelocity, unsigned char downDrumVelocity, unsigned char normalDrumVelocity)
{
    upDrumVelocity_ = upDrumVelocity;
    downDrumVelocity_ = downDrumVelocity;
    normalDrumVelocity_ = normalDrumVelocity;
}

inline void PlayerSettings::setCurrentPattern(unsigned char pattern) {
	currentPattern_ = pattern;
}

inline unsigned char PlayerSettings::getCurrentPattern() {
	return currentPattern_;
}




#endif // PLAYERSETTINGS_H
