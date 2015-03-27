reset
set zeroaxis
set key right bottom
set xlabel "bastl cycles"
set ylabel "step number"

set ytics 1
set xtics 15*32


set key top center outside

set terminal pdfcairo
set output "Stepper.pdf"


set title "General"
plot for [col=2:3] 'Stepper.csv' using 1:col with lines title col







