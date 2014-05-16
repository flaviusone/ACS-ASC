#include <stdio.h>
#include <sys/time.h>

int main(int argc, char** argv) {
	int i, j, k, N = 1000, b_mic = 4, ii, jj, kk;
	double a[1000][1000], b[1000][1000], c[1000][1000], time_spent;
	struct timeval begin, end;
	
	//initializam matricile
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			a[i][j] = rand() % 10;
			b[i][j] = rand() % 10;
			c[i][j] = 0;
		}
	}
	printf("init done\n");

	//calcul nebunie
	gettimeofday(&begin, 0);
	int s = N / b_mic;	
	for (jj = 0; jj < N; jj+= s){
		for (kk = 0; kk < N; kk+=s){
			for (i = 0; i < N; i++){
				for (j = jj; j < (jj + s); j++){
					register double sum = 0;
					for (k = kk; k < kk + s; k++){
						sum  += a[i][k] * b[k][j];
					}
					c[i][j] += sum;
				}
			}
		}	
	}

	gettimeofday(&end, 0);
	
	time_spent = (end.tv_sec - begin.tv_sec) + (double)(end.tv_usec - begin.tv_usec) / 1000000.0;
	printf("time spent = %lf\n", time_spent);
	return 0;
}
