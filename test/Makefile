#CPP_FILES_CLASS = $(wildcard` ../*.cpp)
#OBJ_FILES_CLASS = $(addprefix ../obj/,$(notdir $(CPP_FILES:.cpp=.o)))
CPP_FILES = tst.cpp 
#OBJ_FILES = tst_step.o
#LD_FLAGS=
CC_FLAGS=-c -Wall

#test: $(OBJ_FILES) $(OBJ_FILES_CLASS)
#	g++ $(LD_FLAGS) -o $@ $^

.PHONY : test

test : tst.o ../Tapper.o ../StepDivider.o ../StepMultiplier.o ../StepGenerator.o ../BastlStepper.o
	g++ -o $@ $^
	./test

tst.o : tst.cpp
	g++ $(CC_FLAGS) -o $@ $<

../Tapper.o : ../Tapper.cpp
	g++ $(CC_FLAGS) -o $@ $<

../StepDivider.o : ../StepDivider.cpp
	g++ $(CC_FLAGS) -o $@ $<

../StepMultiplier.o : ../StepMultiplier.cpp
	g++ $(CC_FLAGS) -o $@ $<

../StepGenerator.o : ../StepGenerator.cpp
	g++ $(CC_FLAGS) -o $@ $<
	
../BastlStepper.o : ../BastlStepper.cpp
	g++ $(CC_FLAGS) -o $@ $<


#../obj/%.o: ../%.cpp
#	g++ -I../ -c -o $@ $<
#
#%.o : %.cpp
#	g++ -I../ -c -o $@ $< -I../ISequenceMemory.h

clean:
	-rm -f ../Tapper.o
	-rm -f ../StepDivider.o
	-rm -f ../StepMultiplier.o
	-rm -f ../StepGenerator.o
	-rm -f ../BastlStepper.o
	-rm -f tst.o
	-rm -f test

