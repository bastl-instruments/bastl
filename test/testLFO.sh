rm *.o
rm data/*.csv
rm lfo

g++ -c -DTESTING -Wall -o testLFO.o   testLFO.cpp
g++ -c -DTESTING -Wall -o random.o   ../random.cpp
g++ -c -DTESTING -Wall -o LFO.o   ../LFO.cpp
g++ -c -DTESTING -Wall -o LFOSimple.o   ../LFOSimple.cpp
g++ -c -DTESTING -Wall -o LFORandom.o   ../LFORandom.cpp
g++ -o ./lfo testLFO.o random.o LFO.o LFOSimple.o LFORandom.o
./lfo
cd data
gnuplot lfo.plt

