gnuplot set xlabel 'size'; set ylabel 'time' ; plot 'Nehalem.out' using 1:2 with lines title 'my_dtrmv', 'Nehalem.out' using 1:3 with lines title 'my_dtrmv_imp','Nehalem.out' using 1:4 with lines title 'cblas_dtrmv'


