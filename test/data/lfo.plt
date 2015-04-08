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

set title "Resolution"
plot 'Res0' with lines title col,\
		 'Res1' with lines title col,\
		 'Res2' with lines title col,\
		 'Res3' with lines title col,\
		 'Res4' with lines title col,\
		 'Res5' with lines title col,\
		 'Res6' with lines title col,\
		 'Res7' with lines title col


set title "Smoothness"
plot 'Smooth0' with lines title col,\
		 'Smooth50' with lines title col,\
		 'Smooth100' with lines title col,\
		 'Smooth150' with lines title col,\
		 'Smooth200' with lines title col,\
		 'Smooth250' with lines title col

set title "Smoothness Low"
plot  'Smooth0' with lines title col,\
			'Smooth5' with lines title col,\
			'Smooth10' with lines title col,\
			'Smooth15' with lines title col,\
			'Smooth20' with lines title col,\
			'Smooth25' with lines title col

set title "Flopping"
plot 'Flop0' with lines title col,\
		 'Flop1' with lines title col,\
		 'Flop2' with lines title col,\
		 'Flop3' with lines title col,\
		 'Flop4' with lines title col,\
		 'Flop5' with lines title col,\
		 'Flop6' with lines title col,\
		 'Flop7' with lines title col
		 
		 
set title "Flopping Random"
plot 'FlopRand0' with lines title col,\
		 'FlopRand1' with lines title col,\
		 'FlopRand2' with lines title col,\
		 'FlopRand3' with lines title col,\
		 'FlopRand4' with lines title col,\
		 'FlopRand5' with lines title col,\
		 'FlopRand6' with lines title col,\
		 'FlopRand7' with lines title col
		 
		 
set title "XOR"
plot 'Xor0' with lines title col,\
		 'Xor1' with lines title col,\
		 'Xor2' with lines title col,\
		 'Xor3' with lines title col,\
		 'Xor4' with lines title col,\
		 'Xor5' with lines title col,\
		 'Xor6' with lines title col,\
		 'Xor7' with lines title col
		 
		 
set title "XOR Random"
plot 'XorRand0' with lines title col,\
		 'XorRand1' with lines title col,\
		 'XorRand2' with lines title col,\
		 'XorRand3' with lines title col,\
		 'XorRand4' with lines title col,\
		 'XorRand5' with lines title col,\
		 'XorRand6' with lines title col,\
		 'XorRand7' with lines title col
		 
set title "Folding"
plot 'Fold0' with lines title col,\
		 'Fold1' with lines title col,\
		 'Fold2' with lines title col,\
		 'Fold3' with lines title col,\
		 'Fold4' with lines title col,\
		 'Fold5' with lines title col,\
		 'Fold6' with lines title col,\
		 'Fold7' with lines title col
		 
		 
set title "Folding Random"
plot 'FoldRand0' with lines title col,\
		 'FoldRand1' with lines title col,\
		 'FoldRand2' with lines title col,\
		 'FoldRand3' with lines title col,\
		 'FoldRand4' with lines title col,\
		 'FoldRand5' with lines title col,\
		 'FoldRand6' with lines title col,\
		 'FoldRand7' with lines title col
		 
set title "Overflow"
plot 'Overflow0' with lines title col,\
		 'Overflow1' with lines title col,\
		 'Overflow2' with lines title col,\
		 'Overflow3' with lines title col,\
		 'Overflow4' with lines title col,\
		 'Overflow5' with lines title col,\
		 'Overflow6' with lines title col,\
		 'Overflow7' with lines title col
		 
		 
set title "Overflow Random"
plot 'OverflowRand0' with lines title col,\
		 'OverflowRand1' with lines title col,\
		 'OverflowRand2' with lines title col,\
		 'OverflowRand3' with lines title col,\
		 'OverflowRand4' with lines title col,\
		 'OverflowRand5' with lines title col,\
		 'OverflowRand6' with lines title col,\
		 'OverflowRand7' with lines title col

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



