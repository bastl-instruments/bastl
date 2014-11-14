rm runAverage
rm testAverage.o

g++ -c -o testAverage.o testAverage.cpp
g++ -o runAverage testAverage.o 
./runAverage
