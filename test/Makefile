#CPP_FILES_CLASS = $(wildcard` ../*.cpp)
#OBJ_FILES_CLASS = $(addprefix ../obj/,$(notdir $(CPP_FILES:.cpp=.o)))
CPP_FILES = tst.cpp 
#OBJ_FILES = tst_step.o
#LD_FLAGS=
CC_FLAGS=-c -Wall -g -O0

#test: $(OBJ_FILES) $(OBJ_FILES_CLASS)
#	g++ $(LD_FLAGS) -o $@ $^

.PHONY : check test

check : test radioButtonTest switchesTest
	./test
	./radioButtonTest
	./switchesTest

test : obj/tst.o obj/Tapper.o obj/StepDivider.o obj/StepMultiplier.o obj/StepGenerator.o obj/BastlStepper.o obj/DrumStep.o obj/NoVelocityStepMemory.o obj/Step.o
	g++ -o $@ $^
	./test

switchesTest : obj/switchesTest.o obj/Switches.o obj/HWLayerMock.o
	g++ -o $@ $^
	./switchesTest

obj/Arpeggiator_Test.o : Arpeggiator_test.cpp
	g++ $(CC_FLAGS) -o $@ $<
	
obj/Arpeggiator.o : ../Arpeggiator.cpp
	g++ $(CC_FLAGS) -o $@ $<
	
obj/random.o : ../random.cpp
	g++ $(CC_FLAGS) -o $@ $<

arpeggiatorTest : obj/Arpeggiator_Test.o obj/Arpeggiator.o obj/random.o
	g++ -O0 -o $@ $^
	./arpeggiatorTest

obj/switchesTest.o : tstSwitches.cpp
	g++ $(CC_FLAGS) -o $@ $<

obj/Switches.o : ../Switches.cpp
	g++ $(CC_FLAGS) -o $@ $<

radioButtonTest : obj/radioButtonTest.o obj/RadioButtons.o obj/HWLayerMock.o
	g++ -o $@ $^
	./radioButtonTest

obj/radioButtonTest.o : tstRadioButton.cpp
	g++ $(CC_FLAGS) -o $@ $<

obj/RadioButtons.o : ../RadioButtons.cpp
	g++ $(CC_FLAGS) -o $@ $<

obj/HWLayerMock.o : HWLayerMock.cpp
	g++ $(CC_FLAGS) -o $@ $<

obj/tst.o : tst.cpp
	g++ $(CC_FLAGS) -o $@ $<

obj/Tapper.o : ../Tapper.cpp
	g++ $(CC_FLAGS) -o $@ $<

obj/StepDivider.o : ../../bastl-sequencing/StepDivider.cpp
	g++ $(CC_FLAGS) -o $@ $<

obj/StepMultiplier.o : ../../bastl-sequencing/StepMultiplier.cpp
	g++ $(CC_FLAGS) -o $@ $<

obj/StepGenerator.o : ../../bastl-sequencing/StepGenerator.cpp
	g++ $(CC_FLAGS) -o $@ $<
	
obj/BastlStepper.o : ../../bastl-sequencing/BastlStepper.cpp
	g++ $(CC_FLAGS) -o $@ $<
	
obj/DrumStep.o : ../DrumStep.cpp
	g++ $(CC_FLAGS) -o $@ $<
	
obj/Step.o : ../Step.cpp
	g++ $(CC_FLAGS) -o $@ $<
	
obj/NoVelocityStepMemory.o : ../NoVelocityStepMemory.cpp
	g++ $(CC_FLAGS) -o $@ $<


#../obj/%.o: ../%.cpp
#	g++ -I../ -c -o $@ $<
#
#%.o : %.cpp
#	g++ -I../ -c -o $@ $< -I../ISequenceMemory.h

clean:
	-rm -f obj/*.o
	-rm -f test

