CPP_FILES_CLASS = $(wildcard` ../*.cpp)
OBJ_FILES_CLASS = $(addprefix ../obj/,$(notdir $(CPP_FILES:.cpp=.o)))
#CPP_FILES = tst.cpp
OBJ_FILES = tst_step.o
LD_FLAGS=
CC_FLAGS=-c -Wall

#test: $(OBJ_FILES) $(OBJ_FILES_CLASS)
#	g++ $(LD_FLAGS) -o $@ $^

test : tst.o SRAMHWMock.o ../Step.o ../DrumStep.o DummyStepMemory.o ../FlashStepMemory.o
	g++ -o $@ $^
	./test

SRAMHWMock.o : SRAMHWMock.cpp
	g++ $(CC_FLAGS) -o $@ $<

tst_step.o : tst_step.cpp
	g++ $(CC_FLAGS) -o $@ $<

../Step.o : ../Step.cpp
	g++ $(CC_FLAGS) -o $@ $<

../DrumStep.o : ../DrumStep.cpp
	g++ $(CC_FLAGS) -o $@ $<

DummyStepMemory.o : DummyStepMemory.cpp
	g++ $(CC_FLAGS) -o $@ $<

../FlashStepMemory.o : ../FlashStepMemory.cpp
	g++ $(CC_FLAGS) -o $@ $<


#../obj/%.o: ../%.cpp
#	g++ -I../ -c -o $@ $<
#
#%.o : %.cpp
#	g++ -I../ -c -o $@ $< -I../ISequenceMemory.h

clean:
	rm ../*.o
	rm *.o
	rm test

