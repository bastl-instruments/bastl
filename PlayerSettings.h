#ifndef PLAYERSETTINGS_H
#define PLAYERSETTINGS_H

#include "RackInstrumentDefinitions.h"
#include "DrumStep.h"
#include "BitArrayOperations.h"

class PlayerSettings
{
public:

	enum DrumInstrumentEventType {TRIGGER, GATE};
	enum QuantizationType {_1_64, _1_32, _1_16, _1_8};
	enum MultiplicationType {_1, _2, _4, _8};
	enum PlayerMode {MASTER, SLAVE};

    PlayerSettings();

    QuantizationType getRecordQuantizationType();
    void setRecordQuantizationType(QuantizationType quatizationType);

    unsigned char getDrumInstrumentNote(unsigned char instrumentID);
    void setDrumInstrumentNote(unsigned char instrumentID, unsigned char note);

    DrumInstrumentEventType getDrumInstrumentEventType(unsigned char instrumentID);
    void setDrumInstrumentEventType(unsigned char instrumentID, DrumInstrumentEventType eventType);

    unsigned char getInstrumentChannel(Step::InstrumentType type, unsigned char instrumentID);
    void setInstrumentChannel(Step::InstrumentType type, unsigned char instrumentID, unsigned char channel);

    unsigned char getMIDIVelocityFromDrumVelocity(DrumStep::DrumVelocityType type);
    void setMIDIVelocitiesForDrumVelocities(unsigned char upDrumVelocity, unsigned char downDrumVelocity, unsigned char normalDrumVelocity);

    bool isInstrumentOn(Step::InstrumentType type, unsigned char instrumentID);
    void setInstrumentOn(Step::InstrumentType type, unsigned char instrumentID, bool isOn);

    void setCurrentPattern(unsigned char pattern);
    unsigned char getCurrentPattern();

    bool getDrumInstrumentIndexFromMIDIMessage(unsigned char channel, unsigned char note, unsigned char & drumInstrumentID);

    void setPatternChangedCallback(void (*patternChangedCallback)(unsigned char patternIndex));

    MultiplicationType getMultiplication();
    void setMultiplication(MultiplicationType multiplication);
    void setMultiplicationChangedCallback(void (*multiplicationChangedCallback)(MultiplicationType multiplication));

    unsigned int getBPM();
    void setBPM(unsigned int bpm, bool raiseCallback = true);
    void setBPMChangedCallback(void (*bpmChangedCallback)(unsigned int bpm));

    PlayerMode getPlayerMode();
    void setPlayerMode(PlayerMode playerMode);
    void setPlayerModeChangedCallback(void (*playerModeChangedCallback)(PlayerMode playerMode));

private:
    unsigned char drumInstrumentNotes_[DRUM_INSTRUMENTS];
    unsigned char drumInstrumentEventTypes_;
    unsigned char instrumentChannels_[(DRUM_INSTRUMENTS + MONO_INSTRUMENTS) / 2];
    unsigned char instrumentStatuses_[ALL_INSTRUMENTS_IN_BYTES];
    unsigned char upDrumVelocity_;
    unsigned char downDrumVelocity_;
    unsigned char normalDrumVelocity_;
    unsigned char currentPattern_;
    void (*patternChangedCallback_)(unsigned char patternIndex);
    QuantizationType recordQunatizationType_;
    MultiplicationType multiplication_;
    void (*multiplicationChangedCallback_)(MultiplicationType multiplication);
    unsigned int bpm_;
    void (*bpmChangedCallback_)(unsigned int bpm);
    PlayerMode playerMode_;
    void (*playerModeChangedCallback_)(PlayerMode playerMode);

};

inline PlayerSettings::DrumInstrumentEventType PlayerSettings::getDrumInstrumentEventType(unsigned char instrumentID)
{
	return GETBIT(drumInstrumentEventTypes_, instrumentID) ? GATE : TRIGGER;
}

inline void PlayerSettings::setDrumInstrumentEventType(unsigned char instrumentID, DrumInstrumentEventType eventType)
{
	SETBIT(drumInstrumentEventTypes_, instrumentID, eventType == GATE);
}

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

inline unsigned char PlayerSettings::getCurrentPattern() {
	return currentPattern_;
}

inline void PlayerSettings::setPatternChangedCallback(void (*patternChangedCallback)(unsigned char patternIndex)) {
	patternChangedCallback_ = patternChangedCallback;
}

inline PlayerSettings::QuantizationType PlayerSettings::getRecordQuantizationType() {
	return recordQunatizationType_;
}

inline void PlayerSettings::setRecordQuantizationType(PlayerSettings::QuantizationType quatizationType) {
	recordQunatizationType_ = quatizationType;
}

inline PlayerSettings::MultiplicationType PlayerSettings::getMultiplication() {
	return multiplication_;
}

inline void PlayerSettings::setMultiplication(PlayerSettings::MultiplicationType multiplication) {
	multiplication_ = multiplication;
	if (multiplicationChangedCallback_ != 0) {
		multiplicationChangedCallback_(multiplication_);
	}
}

inline void PlayerSettings::setMultiplicationChangedCallback(void (*multiplicationChangedCallback)(PlayerSettings::MultiplicationType multiplication)) {
	multiplicationChangedCallback_ = multiplicationChangedCallback;
}

inline unsigned int PlayerSettings::getBPM() {
	return bpm_;
}

inline void PlayerSettings::setBPMChangedCallback(void (*bpmChangedCallback)(unsigned int bpm)) {
	bpmChangedCallback_ = bpmChangedCallback;
}

inline PlayerSettings::PlayerMode PlayerSettings::getPlayerMode() {
	return playerMode_;
}

inline void PlayerSettings::setBPM(unsigned int bpm, bool raiseCallback) {
	bpm_ = bpm;
	if (raiseCallback && bpmChangedCallback_) {
		bpmChangedCallback_(bpm);
	}
}

inline void PlayerSettings::setPlayerMode(PlayerSettings::PlayerMode playerMode) {
	playerMode_ = playerMode;
	if (playerModeChangedCallback_) {
		playerModeChangedCallback_(playerMode_);
	}
}

inline void PlayerSettings::setPlayerModeChangedCallback(void (*playerModeChangedCallback)(PlayerSettings::PlayerMode playerMode)) {
	playerModeChangedCallback_ = playerModeChangedCallback;
}

inline unsigned char PlayerSettings::getMIDIVelocityFromDrumVelocity(DrumStep::DrumVelocityType type) {
	return normalDrumVelocity_;
}

#endif // PLAYERSETTINGS_H
