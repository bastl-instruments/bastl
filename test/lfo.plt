reset
set zeroaxis
set key right bottom
set xlabel "Step"
set ylabel "lfo output"


set terminal png
set output "lfo.png"
plot 'lfo.csv' notitle with lines


