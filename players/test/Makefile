CPP_FILES_CLASS = $(wildcard` ../*.cpp)
OBJ_FILES_CLASS = $(addprefix ../obj/,$(notdir $(CPP_FILES:.cpp=.o)))
#CPP_FILES = tst.cpp
OBJ_FILES = tst_step.o
LD_FLAGS=
CC_FLAGS=-c -Wall

#test: $(OBJ_FILES) $(OBJ_FILES_CLASS)
#	g++ $(LD_FLAGS) -o $@ $^

test : tst.o MIDICommandProcessorMock.o ../Player.o ../PlayerSettings.o ../../data/DrumStep.o ../../data/DummyStepMemory.o ../../data/MIDICommand.o ../../data/Step.o
	g++ -o $@ $^
	./test

tst.o : tst.cpp
	g++ $(CC_FLAGS) -o $@ $<

MIDICommandProcessorMock.o : MIDICommandProcessorMock.cpp
	g++ $(CC_FLAGS) -o $@ $<

../Player.o : ../Player.cpp
	g++ $(CC_FLAGS) -o $@ $<

../PlayerSettings.o : ../PlayerSettings.cpp
	g++ $(CC_FLAGS) -o $@ $<

../../data/DrumStep.o : ../../data/DrumStep.cpp
	g++ $(CC_FLAGS) -o $@ $<

../../data/DummyStepMemory.o : ../../data/DummyStepMemory.cpp
	g++ $(CC_FLAGS) -o $@ $<

../../data/Step.o : ../../data/Step.cpp
	g++ $(CC_FLAGS) -o $@ $<

../../data/MIDICommand.o : ../../data/MIDICommand.cpp
	g++ $(CC_FLAGS) -o $@ $<

#../obj/%.o: ../%.cpp
#	g++ -I../ -c -o $@ $<
#
#%.o : %.cpp
#	g++ -I../ -c -o $@ $< -I../ISequenceMemory.h

clean:
	rm ../*.o
	rm *.o
	rm ../../data/*.o
	rm test

