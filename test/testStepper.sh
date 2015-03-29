rm *.o
rm data/*.csv
rm stepper

g++ -c -DTESTING -Wall -o stepperTest.o   testStepper.cpp
g++ -o ./stepper stepperTest.o 
./stepper
cd data
gnuplot stepper.plt
