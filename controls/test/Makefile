CPP_FILES_CLASS = $(wildcard` ../*.cpp)
OBJ_FILES_CLASS = $(addprefix ../obj/,$(notdir $(CPP_FILES:.cpp=.o)))
#CPP_FILES = tst.cpp
OBJ_FILES = tst_step.o
LD_FLAGS=
CC_FLAGS=-c -Wall

#test: $(OBJ_FILES) $(OBJ_FILES_CLASS)
#	g++ $(LD_FLAGS) -o $@ $^

test : tst.o HWLayerMock.o ../RadioButtons.o ../Switches.o
	g++ -o $@ $^
	./test

tst.o : tst.cpp
	g++ $(CC_FLAGS) -o $@ $<

HWLayerMock.o : HWLayerMock.cpp
	g++ $(CC_FLAGS) -o $@ $<

../RadioButtons.o : ../RadioButtons.cpp
	g++ $(CC_FLAGS) -o $@ $<

../Switches.o : ../Switches.cpp
	g++ $(CC_FLAGS) -o $@ $<

clean:
	rm ../*.o
	rm *.o
	rm ../../data/*.o
	rm test

