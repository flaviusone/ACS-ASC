#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	int i, j, k, N = 1000;
	//double a[1000][1000], b[1000][1000], c[1000][1000];
	double time_spent;
	double **a, **b, **c;

	struct timeval begin, end;
	
	//initializam matricile
	a = (double**) malloc (1000 * sizeof(double*));
	b = (double**) malloc (1000 * sizeof(double*));
	c = (double**) malloc (1000 * sizeof(double*));

	for (i = 0; i < N; i++) {
		a[i] = (double*) malloc(1000 * sizeof(double));
		b[i] = (double*) malloc(1000 * sizeof(double));
		c[i] = (double*) malloc(1000 * sizeof(double));
		for (j = 0; j < N; j++) {
			a[i][j] = rand() % 100;
			b[i][j] = rand() % 100;
			c[i][j] = 0;
		}
	}
	printf("init done\n");

	//calcul nebunie
	gettimeofday(&begin, 0);
	
	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			c[i][j] = 0.0;
			for (k = 0; k < N; k++){
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}

	gettimeofday(&end, 0);
	
	time_spent = (end.tv_sec - begin.tv_sec) + (double)(end.tv_usec - begin.tv_usec) / 1000000.0;
	printf("time spent = %lf\n", time_spent);
	printf("%d %d %d\n", c[0][0], c[0][1], c[0][2]);
	return 0;
}
