CPP_FILES_CLASS = $(wildcard` ../*.cpp)
OBJ_FILES_CLASS = $(addprefix ../obj/,$(notdir $(CPP_FILES:.cpp=.o)))
#CPP_FILES = tst.cpp
OBJ_FILES = tst_step.o
LD_FLAGS=
CC_FLAGS=-c -Wall -fno-strict-overflow

#test: $(OBJ_FILES) $(OBJ_FILES_CLASS)
#	g++ $(LD_FLAGS) -o $@ $^

test : tst.o SteppingHWLayerMock.o ../Stepper.o ../MIDIStepper.o
	g++ -o $@ $^
	./test

tst.o : tst.cpp
	g++ $(CC_FLAGS) -o $@ $<

SteppingHWLayerMock.o : SteppingHWLayerMock.cpp
	g++ $(CC_FLAGS) -o $@ $<

../Stepper.o : ../Stepper.cpp
	g++ $(CC_FLAGS) -o $@ $<

../MIDIStepper.o : ../MIDIStepper.cpp
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

