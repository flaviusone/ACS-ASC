#include <stdio.h>
#include <sys/time.h>

int main(int argc, char** argv) {
	int i, j, k, N = 1000;
	double a[1000][1000], b[1000][1000], c[1000][1000], time_spent;
	struct timeval begin, end;
	
	//initializam matricile
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			a[i][j] = rand() % 100;
			b[i][j] = rand() % 100;
		}
	}
	printf("init done\n");

	//calcul nebunie
	gettimeofday(&begin, 0);
	
	for (i = 0; i < N; i++){
		double *orig_pa = &a[i][0];
		for (j = 0; j < N; j++){
			double *pa = orig_pa;
			double *pb = &b[0][j];
			register double suma = 0;
			for (k = 0; k < N; k++){
				suma += *pa * *pb;
				pa++;
				pb += N;
			}
			c[i][j] = suma;
		}
	}

	gettimeofday(&end, 0);
	
	time_spent = (end.tv_sec - begin.tv_sec) + (double)(end.tv_usec - begin.tv_usec) / 1000000.0;
	printf("time spent = %lf\n", time_spent);
	return 0;
}
