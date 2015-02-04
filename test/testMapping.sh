rm runMapping
rm testMapping.o

g++ -DTESTING -c -o testMapping.o testMapping.cpp
g++ -o runMapping testMapping.o
./runMapping > mapping.csv
gnuplot mapping.plt
