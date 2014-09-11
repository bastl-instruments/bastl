#CPP_FILES_CLASS = $(wildcard` ../*.cpp)
#OBJ_FILES_CLASS = $(addprefix ../obj/,$(notdir $(CPP_FILES:.cpp=.o)))
#CPP_FILES = tst.cpp
#OBJ_FILES = tst_step.o
#LD_FLAGS=
CC_FLAGS=-c -Wall

#test: $(OBJ_FILES) $(OBJ_FILES_CLASS)
#	g++ $(LD_FLAGS) -o $@ $^

test : tst.o ../Tapper.o
	g++ -o $@ $^
	./test

tst.o : tst.cpp
	g++ $(CC_FLAGS) -o $@ $<

../Tapper.o : ../Tapper.cpp
	g++ $(CC_FLAGS) -o $@ $<

#../obj/%.o: ../%.cpp
#	g++ -I../ -c -o $@ $<
#
#%.o : %.cpp
#	g++ -I../ -c -o $@ $< -I../ISequenceMemory.h

clean:
	rm ../Tapper.o
	rm tst.o
	rm test

