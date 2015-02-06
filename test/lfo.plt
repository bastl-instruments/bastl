reset
set zeroaxis
set key right bottom
set xlabel "Step"
set ylabel "lfo output"

set ytics 16
set mytics 16

set grid ytics mytics

set key top left

set terminal pdfcairo
set output "lfo.pdf"
plot for [col=2:5] 'lfo.csv' using 1:col with linespoints


