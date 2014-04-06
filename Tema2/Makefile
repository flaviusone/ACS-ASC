OBJ=cblas_dtrmv.o my_dtrmv.o my_dtrmv_imp.o
TARGET= cblas_dtrmv_opteron cblas_dtrmv_nehalem cblas_dtrmv_quad my_dtrmv_imp_opteron my_dtrmv_imp_nehalem my_dtrmv_imp_quad my_dtrmv_opteron my_dtrmv_nehalem my_dtrmv_uad
OFLAGS=-O3 -floop-block -funroll-loops -fno-strict-aliasing

all: build

build:
	gcc -o my_dtrmv_opteron my_dtrmv.c -lcblas -latlas -L/opt/tools/libraries/atlas/3.10.1-opteron-gcc-4.4.6/lib -I/opt/tools/libraries/atlas/3.10.1-opteron-gcc-4.4.6/include
	gcc -o my_dtrmv_imp_opteron my_dtrmv_imp.c $(OFLAGS) -lcblas -latlas -L/opt/tools/libraries/atlas/3.10.1-opteron-gcc-4.4.6/lib -I/opt/tools/libraries/atlas/3.10.1-opteron-gcc-4.4.6/include
	gcc -o cblas_dtrmv_opteron cblas_dtrmv.c -lcblas -latlas -L/opt/tools/libraries/atlas/3.10.1-opteron-gcc-4.4.6/lib -I/opt/tools/libraries/atlas/3.10.1-opteron-gcc-4.4.6/include

	gcc -o my_dtrmv_nehalem my_dtrmv.c -lcblas -latlas -L/opt/tools/libraries/atlas/3.10.1-nehalem-gcc-4.4.6/lib -I/opt/tools/libraries/atlas/3.10.1-nehalem-gcc-4.4.6/include
	gcc -o my_dtrmv_imp_nehalem my_dtrmv_imp.c $(OFLAGS) -lcblas -latlas -L/opt/tools/libraries/atlas/3.10.1-nehalem-gcc-4.4.6/lib -I/opt/tools/libraries/atlas/3.10.1-nehalem-gcc-4.4.6/include
	gcc -o cblas_dtrmv_nehalem cblas_dtrmv.c -lcblas -latlas -L/opt/tools/libraries/atlas/3.10.1-nehalem-gcc-4.4.6/lib -I/opt/tools/libraries/atlas/3.10.1-nehalem-gcc-4.4.6/include

	gcc -o my_dtrmv_quad my_dtrmv.c -lcblas -latlas -L/opt/tools/libraries/atlas/3.10.1-quad-gcc-4.4.6/lib -I/opt/tools/libraries/atlas/3.10.1-quad-gcc-4.4.6/include
	gcc -o my_dtrmv_imp_quad my_dtrmv_imp.c $(OFLAGS) -lcblas -latlas -L/opt/tools/libraries/atlas/3.10.1-quad-gcc-4.4.6/lib -I/opt/tools/libraries/atlas/3.10.1-quad-gcc-4.4.6/include
	gcc -o cblas_dtrmv_quad cblas_dtrmv.c -lcblas -latlas -L/opt/tools/libraries/atlas/3.10.1-quad-gcc-4.4.6/lib -I/opt/tools/libraries/atlas/3.10.1-quad-gcc-4.4.6/include

drawplot-nehalem:
	gnuplot -persist -e "set xlabel 'size'; set ylabel 'time' ;plot 'Nehalem.out' using 1:2 with lines title 'my_dtrmv', 'Nehalem.out' using 1:3 with lines title 'my_dtrmv_imp','Nehalem.out' using 1:4 with lines title 'cblas_dtrmv'"
drawplot-opteron:
	gnuplot -persist -e "set output 'Opteron.png' ; set xlabel 'size'; set ylabel 'time' ;plot 'Opteron.out' using 1:2 with lines title 'my_dtrmv', 'Opteron.out' using 1:3 with lines title 'my_dtrmv_imp','Opteron.out' using 1:4 with lines title 'cblas_dtrmv'"
drawplot-quad:
	gnuplot -persist -e "set output 'Quad.png' ; set xlabel 'size'; set ylabel 'time' ;plot 'Quad.out' using 1:2 with lines title 'my_dtrmv', 'Quad.out' using 1:3 with lines title 'my_dtrmv_imp','Quad.out' using 1:4 with lines title 'cblas_dtrmv'"
drawplot-all:
	gnuplot -persist -e "set output 'All.png' ; set xlabel 'size'; set ylabel 'time' \
	;plot 'All.out' using 1:2 with lines title 'my_dtrmv_Nehalem', 'All.out' using 1:3 with lines title 'my_dtrmv_imp_Nehalem', \
	'All.out' using 1:4 with lines title 'cblas_dtrmv_Nehalem','All.out' using 1:5 with lines title 'my_dtrmv_Opteron', \
	'All.out' using 1:6 with lines title 'my_dtrmv_imp_Opteron','All.out' using 1:7 with lines title 'cblas_dtrmv_Opteron',\
	 'All.out' using 1:8 with lines title 'my_dtrmv_Quad', 'All.out' using 1:9 with lines title 'my_dtrmv_imp_Quad', \
	'All.out' using 1:10 with lines title 'cblas_dtrmv_Quad'"


clean:
	rm -rf $(OBJ) $(TARGET) *~
