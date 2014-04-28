gnuplot set xlabel 'size'; set ylabel 'time' ;plot 'Nehalem.out' using 1:2 with lines title 'my_dtrmv', 'Nehalem.out' using 1:3 with lines title 'my_dtrmv_imp','Nehalem.out' using 1:4 with lines title 'cblas_dtrmv'


set output 'Opteron.png' ; set xlabel 'size'; set ylabel 'time' ;plot 'Opteron.out' using 1:2 with lines title 'my_dtrmv', 'Opteron.out' using 1:3 with lines title 'my_dtrmv_imp','Opteron.out' using 1:4 with lines title 'cblas_dtrmv'

set output 'Quad.png' ; set xlabel 'size'; set ylabel 'time' ;plot 'Quad.out' using 1:2 with lines title 'my_dtrmv', 'Quad.out' using 1:3 with lines title 'my_dtrmv_imp','Quad.out' using 1:4 with lines title 'cblas_dtrmv'
