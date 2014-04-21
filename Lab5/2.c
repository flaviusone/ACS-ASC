#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

// alocare in zona continua de memorie
double **allocMat(int w, int h)
{
    double **mat;
    double *buf;
    buf = (double *) calloc(h * w, sizeof(double));
    int i;
    mat = (double **) calloc(h, sizeof(double *));
    for (i = 0; i < h; i++)
        mat[i] = buf + i * w;
    return mat;
}

int main(int argc, char** argv){

	struct timeval start,finish;
	double t;
	double **a,**b,**c;
	int i,j,k,N;
	N = 1000;
	a = allocMat(1000,1000);
	b = allocMat(1000,1000);
	c = allocMat(1000,1000);

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			a[i][j] = rand() % 100;
			b[i][j] = rand() % 100;
			c[i][j] = 0;
		}
	}

	/*------------------------ CAZ i-j-k ------------------------*/
	gettimeofday(&start,0);

	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			c[i][j] = 0.0;
			for (k = 0; k < N; k++){
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}

	gettimeofday(&finish,0);

	t = (finish.tv_sec - start.tv_sec) + (double)(finish.tv_usec - start.tv_usec) / 1000000.0;
	printf("Timp CAZ i-j-k = %lf\n", t);
	/*------------------------ CAZ i-j-k ------------------------*/

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			c[i][j]=0;


	/*------------------------ CAZ i-k-j ------------------------*/
	gettimeofday(&start,0);

	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			c[i][j] = 0.0;
			for (k = 0; k < N; k++){
				c[i][k] += a[i][j] * b[k][j];
			}
		}
	}

	gettimeofday(&finish,0);

	t = (finish.tv_sec - start.tv_sec) + (double)(finish.tv_usec - start.tv_usec) / 1000000.0;
	printf("Timp CAZ i-k-j = %lf\n", t);
	/*------------------------ CAZ i-k-j ------------------------*/

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			c[i][j]=0;

			/*------------------------ CAZ j-k-i ------------------------*/
	gettimeofday(&start,0);

	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			c[i][j] = 0.0;
			for (k = 0; k < N; k++){
				c[k][j] += a[k][k] * b[i][j];
			}
		}
	}

	gettimeofday(&finish,0);

	t = (finish.tv_sec - start.tv_sec) + (double)(finish.tv_usec - start.tv_usec) / 1000000.0;
	printf("Timp CAZ j-k-i = %lf\n", t);
	/*------------------------ CAZ j-k-i ------------------------*/

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			c[i][j]=0;
	return 0;
}
