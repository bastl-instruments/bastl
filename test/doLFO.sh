g++ -c -Wall -o lfo.o   LFO-Test.cpp
g++ -o ./lfo lfo.o random.o
./lfo > lfo.csv
gnuplot lfo.plt
eog lfo.png &
