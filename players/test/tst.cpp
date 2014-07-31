#include <iostream>
#include "../PlayerSettings.h"
#include "../Player.h"
#include "../../data/InstrumentDefinitions.h"
#include "../../data/test/DummyStepMemory.h"
#include "MIDICommandProcessorMock.h"

int main( int argc, const char* argv[] ) {
    PlayerSettings settings;
    printf("Testing PlayerSettings\n");

    settings.setInstrumentChannel(InstrumentTypes::DRUM, 19, 0);
    printf("\tTest 1 - Set DRUM instrument channel \t\t%s\n", settings.getInstrumentChannel(InstrumentTypes::DRUM, 10) == 0 ? "OK" : "Error");
    settings.setInstrumentChannel(InstrumentTypes::MONO, 3, 15);
    printf("\tTest 2 - Set MONO instrument channel \t\t%s\n", settings.getInstrumentChannel(InstrumentTypes::MONO, 2) == 15 ? "OK" : "Error");
    settings.setDrumInstrumentNote(19, 120);
    printf("\tTest 3 - Set DRUM instrument note \t\t%s\n", settings.getDrumInstrumentNote(19) == 120 ? "OK" : "Error");

    printf("\tTest 4 - Default DRUM instrument status \t\t%s\n", settings.isInstrumentOn(InstrumentTypes::DRUM, 19) ? "OK" : "Error");
    printf("\tTest 5 - Default MONO instrument status \t\t%s\n", settings.isInstrumentOn(InstrumentTypes::MONO, 0) ? "OK" : "Error");

    settings.setInstrumentOn(InstrumentTypes::DRUM, 19, false);
    printf("\tTest 6 - Set DRUM instrument status \t\t%s\n", settings.isInstrumentOn(InstrumentTypes::DRUM, 19) ? "Error" : "OK");

    settings.setInstrumentOn(InstrumentTypes::MONO, 0, false);
    printf("\tTest 7 - Set MONO instrument status \t\t%s\n", settings.isInstrumentOn(InstrumentTypes::MONO, 0) ? "Error" : "OK");

    settings.setMIDIVelocitiesForDrumVelocities(10, 20, 30);
    printf("\tTest 8  - Set map DRUM to MIDI velocities (Up) \t\t%s\n", settings.getMIDIVelocityFromDrumVelocity(DrumStep::UP) == 10 ? "OK" : "Error");
    printf("\tTest 9  - Set map DRUM to MIDI velocities (Down) \t\t%s\n", settings.getMIDIVelocityFromDrumVelocity(DrumStep::DOWN) == 20 ? "OK" : "Error");
    printf("\tTest 10 - Set map DRUM to MIDI velocities (Normal) \t\t%s\n", settings.getMIDIVelocityFromDrumVelocity(DrumStep::NORMAL) == 30 ? "OK" : "Error");

    printf("\nTesting MIDI command processor MOCK\n");
    MIDICommandProcessorMock mock;
    printf("\tTest 11 - Checking initialized On counters \t\t%s\n", mock.getNoteOffCountForChannel(5) == 0 ? "OK" : "Error");
    printf("\tTest 12 - Checking initialized Off counters \t\t%s\n", mock.getNoteOnCountForChannel(10) == 0 ? "OK" : "Error");
    MIDICommand onCommand(MIDICommand::NOTEON, 0, 0, 0);
    onCommand.setChannel(10);
    MIDICommand offCommand(MIDICommand::NOTEOFF, 0, 0, 0);
    offCommand.setChannel(5);
    mock.SendCommand(onCommand);
    mock.SendCommand(onCommand);
    mock.SendCommand(offCommand);
    printf("\tTest 13 - Checking used On counters \t\t%s\n", mock.getNoteOffCountForChannel(5) == 1 ? "OK" : "Error");
    printf("\tTest 14 - Checking used Off counters \t\t%s\n", mock.getNoteOnCountForChannel(10) == 2 ? "OK" : "Error");
    mock.clear();
    printf("\tTest 15 - Checking cleared On counters \t\t%s\n", mock.getNoteOffCountForChannel(5) == 0 ? "OK" : "Error");
    printf("\tTest 16 - Checking cleared Off counters \t\t%s\n", mock.getNoteOnCountForChannel(10) == 0 ? "OK" : "Error");


    printf("\nTesting Player\n");

    DummyStepMemory stepMemory;
    DrumStep::DrumVelocityType substeps[4] = {DrumStep::NORMAL, DrumStep::OFF, DrumStep::OFF, DrumStep::OFF};
    DrumStep::DrumVelocityType substeps2[4] = {DrumStep::OFF, DrumStep::OFF, DrumStep::OFF, DrumStep::OFF};
    //DrumStep::DrumVelocityType substeps3[4] = {DrumStep::NORMAL, DrumStep::OFF, DrumStep::OFF, DrumStep::NORMAL};
    for (int i = 0; i < 8; i++) {
    	stepMemory.setDrumStep(0, 0, i, DrumStep(true, false, substeps));
    }
    for (int i = 8; i < 64; i++) {
    	stepMemory.setDrumStep(0, 0, i, DrumStep(false, false, substeps2));
    }
    //stepMemory.setDrumStep(1, 0, 1, DrumStep(false, false, substeps2));
    //stepMemory.setDrumStep(1, 0, 2, DrumStep(true, true, substeps2));
    //stepMemory.setDrumStep(1, 0, 3, DrumStep(true, false, substeps3));
    Player player(& stepMemory, & mock, & settings);

    //First step
    for (int i = 0; i < 64; i++) {
    	player.stepFourth();
    	/*printf(" %d NOTE ONs: %d NOTE OFFs %d Curent step: %d\n",
    			i,
    			mock.getNoteOnCountForChannel(0),
    			mock.getNoteOffCountForChannel(0),
    			player.getCurrentInstrumentStep(0));*/
    }

    printf("\tTest 17 - Two instruments playing one step \t\t%s\n", mock.getNoteOnCountForChannel(0) == 6 &&
                                                                    mock.getNoteOffCountForChannel(0) == 4 ? "OK" : "Error");

    //Second step is disactivated playing third that is empty
    player.stepFourth();
    player.stepFourth();
    player.stepFourth();
    player.stepFourth();

    printf("\tTest 18 - One inactive + one muted \t\t%s\n", mock.getNoteOnCountForChannel(0) == 6 &&
                                                            mock.getNoteOffCountForChannel(0) == 6 ? "OK" : "Error");

    //Playing fourth
    player.stepFourth();
    player.stepFourth();
    player.stepFourth();
    player.stepFourth();

    printf("\tTest 20 - Next one substep to noteOff \t\t%s\n", mock.getNoteOnCountForChannel(0) == 8 &&
                                                               mock.getNoteOffCountForChannel(0) == 7 ? "OK" : "Error");

    player.stepFourth();

    printf("\tTest 19 - Next one active step \t\t%s\n", mock.getNoteOnCountForChannel(0) == 8 &&
                                                        mock.getNoteOffCountForChannel(0) == 8 ? "OK" : "Error");
}
