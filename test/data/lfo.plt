reset
set zeroaxis
set key right bottom
set xlabel "number of time units"
set ylabel "lfo output"

set ytics 64
#set mytics 16

set grid ytics mytics
#set xtics out 100
#set mxtics 10

set key top center outside

set terminal pdfcairo
set output "LFO.pdf"


set title "Frequency"
plot 'Freq1' with lines title col,'Freq2' with lines title col, 'Freq3' with lines title col

set title "Waveform"
plot 'Waveform1' with lines title col,'Waveform2' with lines title col,'Waveform3' with lines title col

set title "Smoothness"
plot 'Smooth0' with lines title col,'Smooth50' with lines title col,'Smooth100' with lines title col,'Smooth150' with lines title col,'Smooth200' with lines title col,'Smooth250' with lines title col

set title "Smoothness Low"
plot 'Smooth0' with lines title col,'Smooth5' with lines title col,'Smooth10' with lines title col,'Smooth15' with lines title col,'Smooth20' with lines title col,'Smooth25' with lines title col

#set title "Overflow"
#plot for [col=2:5] 'lfoTimeOverflow.csv' using 1:col with lines title col


#set title "Waveforms"
#plot for [col=2:5] 'lfoWaveforms.csv' using 1:col with lines title col


#set title "Resolution"
#plot for [col=2:5] 'lfoResolution.csv' using 1:col with lines title col

#set title "XOR"
#plot for [col=2:5] 'lfoXOR.csv' using 1:col with lines title col

#set title "Flop"
#plot for [col=2:5] 'lfoFlop.csv' using 1:col with lines title col

#set title "Threshold"
#plot for [col=2:5] 'lfoThres.csv' using 1:col with lines title col

#set title "Random"
#plot for [col=2:5] 'lfoRandom.csv' using 1:col with lines title col

#set title "set step"
#plot for [col=2:5] 'lfoSetStep.csv' using 1:col with lines title col





#set y2tics 10000

#set terminal pdfcairo
#set output "lfoInternal.pdf"

#plot for [col=2:4] 'lfoInternal.csv' using 1:col with lines title col axes x1y2 lw 2,\
#	for [col=5:6] 'lfoInternal.csv' using 1:col with lines title col axes x1y1



