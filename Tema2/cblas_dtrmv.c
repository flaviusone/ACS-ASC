#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
// #include "cblas.h"

void print_matrix(double *mat,unsigned int n){
	unsigned int i,j;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			printf("%5.2f\t",mat[j+i*n] );
		}
		printf("\n");
	}
}

double* create_matrix(unsigned int n){
	double *mat;
	unsigned int i,j;
	mat = calloc(n*n, sizeof(double));

	srand(time(NULL));
	//o umplu  cu 1 superior triunghiular
	for(i=0;i<n;i++){
		for(j=i;j<n;j++){
			mat[j+i*n]=rand();
		}
	}

	return mat;
}
void my_dtrmv(const char* row, const char* uplo, const char* trans, const char*
	diag, unsigned int n, double* a, unsigned int lda, double* x,int incx){

	double *out = malloc(n*sizeof(double));
	unsigned int i,j;

	// Fac inmultirea
	for ( i = 0; i < n; i++ )
	{
		out[i] = 0;
		for ( j = i; j < n; j++ )
			out[i] += a[j+i*n] * x[j];
	}

	for(i = 0; i < n ;i++){
		x[i] = out[i];
	}

	return;
}


void my_dtrmv_imp(const char* row, const char* uplo, const char* trans, const
	char* diag, unsigned int n, double* a, unsigned int lda, double* x,int incx){

	double *out = malloc(n*sizeof(double));
	unsigned int i,j;

	for ( i = 0; i < n; i++ )
	{
		double *pa = a+n*i+i;
		double *pb = &x[i];
		register double suma=0;
		for ( j = i; j < n; j++ ){
			suma += *pa * *pb;
			pb++;
			pa++;
		}
		out[i] = suma;
	}

	for(i = 0; i < n ;i++){
		x[i] = out[i];
	}

	return;
}

void print_vec(double* vec,unsigned int n){
	unsigned int i;
	printf("\n");
	// Printez rezutlat
	for(i=0;i<n;i++)
		printf("%5.2f ",vec[i]);
	printf("\n\n");
}

int main(int argc,const char* argv[]){
	unsigned int i;
	unsigned int n = atoi(argv[1]);
	struct timeval start,finish;
	double t;

	// aloc si initializez matrice
	double* mat = create_matrix(n);

	srand(time(NULL));
	// Initializez vector
	double *vec = malloc(n*sizeof(double));
	for(i=0;i<n;i++){
		vec[i]=rand();
	}

	/*------------------------------clblas_dtrmv------------------------------*/
	gettimeofday(&start,0);

	// Fac inmultirea - cblas
	cblas_dtrmv(CblasRowMajor, CblasUpper, CblasNoTrans, CblasNonUnit, n, mat, n, vec, 1);


	gettimeofday(&finish,0);

	t= (finish.tv_sec - start.tv_sec) + (double)(finish.tv_usec - start.tv_usec)
	 / 1000000.0;
	printf("Timp cblas_dtrmv at n=%u = %lf\n", n, t);
	/*------------------------------clblas_dtrmv------------------------------*/


	return 0;
}
