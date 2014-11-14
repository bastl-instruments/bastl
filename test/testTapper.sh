rm runTapper
rm testTapper.o
rm tapper.o

g++ -c -o testTapper.o testTapper.cpp
g++ -c -o tapper.o ../Tapper.cpp
g++ -o runTapper testTapper.o tapper.o
./runTapper
