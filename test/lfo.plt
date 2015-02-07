reset
set zeroaxis
set key right bottom
set xlabel "Timestamp"
set ylabel "lfo output"

set ytics 64
#set mytics 16

set grid ytics mytics
set xtics out 1000

set key top center outside

set terminal pdfcairo
set output "lfo.pdf"
plot for [col=2:5] 'lfo.csv' using 1:col with lines title col


