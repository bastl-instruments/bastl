rm *.o

g++ -c -DTESTING -Wall -o lfoTest.o   LFO-Test.cpp
g++ -c -DTESTING -Wall -o random.o   ../random.cpp
g++ -c -DTESTING -Wall -o lfo.o   ../lfoExtended.cpp
g++ -o ./lfo lfo.o random.o lfoTest.o
./lfo > lfo.csv
gnuplot lfo.plt

