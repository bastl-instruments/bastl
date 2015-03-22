set xrange [0:256]
set xtics 64
set key outside right

set terminal pdfcairo
set output "mapping.pdf"

plot "mapping.csv" using 1:2 with lines,\
	"mapping.csv" using 1:3 with lines,\
"mapping.csv" using 1:4 with lines,\
"mapping.csv" using 1:5 with lines,\
"mapping.csv" using 1:6 with lines,\
"mapping.csv" using 1:7 with lines,\
"mapping.csv" using 1:8 with lines,\
"mapping.csv" using 1:9 with lines
