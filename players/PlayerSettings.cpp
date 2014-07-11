#include "PlayerSettings.h"

PlayerSettings::PlayerSettings() : upDrumVelocity_(253), downDrumVelocity_(64), normalDrumVelocity_(128)
{
    for (unsigned char i = 0; i < DRUM_INSTRUMENTS; i++) {
        drumInstrumentNotes_[i] = 0;
    }
    for (unsigned char i = 0; i < (DRUM_INSTRUMENTS + MONO_INSTRUMENTS) / 2; i++) {
        instrumentChannels_[i] = 0;
    }
    for (unsigned char i = 0; i < ALL_INSTRUMENTS_IN_BYTES; i++) {
        instrumentStatuses_[i] = ~0;
    }
}

void PlayerSettings::setInstrumentOn(InstrumentTypes::InstrumentType type, unsigned char instrumentID, bool isOn)
{
    unsigned char position = (type == InstrumentTypes::MONO) ? 20 : 0 + instrumentID;
    if (isOn) {
        instrumentStatuses_[position / 8] = instrumentStatuses_[position / 8] | (1 << (position % 8));
    } else {
        instrumentStatuses_[position / 8] = instrumentStatuses_[position / 8] & ~(1 << (position % 8));
    }

}

bool PlayerSettings::isInstrumentOn(InstrumentTypes::InstrumentType type, unsigned char instrumentID)
{
    unsigned char position = (type == InstrumentTypes::MONO) ? 20 : 0 + instrumentID;
    unsigned char value = instrumentStatuses_[position / 8];
    return (((1 << (position % 8)) & value) >> (position % 8) == 1);
}

unsigned char PlayerSettings::getInstrumentChannel(InstrumentTypes::InstrumentType type, unsigned char instrumentID) {
    unsigned char position = (type == InstrumentTypes::MONO) ? 20 : 0 + instrumentID;
    unsigned char instrumentShift = ((position % 2) * 4);

    // we have only 16 channels so one byte contains information for two instruments
    return ((15 << instrumentShift) & instrumentChannels_[position / 2]) >> instrumentShift;

}

void PlayerSettings::setInstrumentChannel(InstrumentTypes::InstrumentType type, unsigned char instrumentID, unsigned char channel)
{
    unsigned char position = (type == InstrumentTypes::MONO) ? 20 : 0 + instrumentID;
    unsigned char instrumentShift = ((position % 2) * 4);

    // we have only 16 channels so one byte contains information for two instruments
    instrumentChannels_[position / 2] = instrumentChannels_[position / 2] | channel << instrumentShift;
}



unsigned char PlayerSettings::getMIDIVelocityFromDrumVelocity(DrumStep::DrumVelocityType type) {
    switch (type) {
        case DrumStep::UP:
            return upDrumVelocity_;
        break;
        case DrumStep::DOWN:
            return downDrumVelocity_;
        break;
        case DrumStep::NORMAL:
            return normalDrumVelocity_;
        break;
        default:
            return 0;
    }
}
