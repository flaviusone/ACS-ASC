#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
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

int main(int argc,const char* argv[]){
	unsigned int i,j;
	unsigned int n = atoi(argv[1]);
	struct timeval start,finish;
	double t;

	// aloc si initializez matrice
	double* mat = create_matrix(n);

	srand(time(NULL));
	// Initializez vector
	double *vec = malloc(n*sizeof(double));
	for(i=0;i<n;i++)
		vec[i]=rand();

	/*------------------------------FUCNTIA MEA------------------------------*/
	gettimeofday(&start,0);

	// Fac inmultirea mea



	gettimeofday(&finish,0);

	t= (finish.tv_sec - start.tv_sec) + (double)(finish.tv_usec - start.tv_usec)
	 / 1000000.0;
	printf("Timp Functia mea = %lf\n", t);
	/*------------------------------FUCNTIA MEA------------------------------*/

	my_dtrmv()

	/*------------------------------FUNCTIE BLAS------------------------------*/
	gettimeofday(&start,0);

	// Fac inmultirea - cblas
	// cblas_dtrmv(CblasRowMajor, CblasUpper, CblasNoTrans, CblasNonUnit, n, mat, n, vec, 1);


	gettimeofday(&finish,0);

	t= (finish.tv_sec - start.tv_sec) + (double)(finish.tv_usec - start.tv_usec)
	 / 1000000.0;
	printf("Timp Functie BLAS = %lf\n", t);
	/*------------------------------FUNCTIE BLAS------------------------------*/

	print_matrix(mat,n);

	// Fac inmultirea
	// for ( i = 0; i < m; i++ )
	// {
	// 	out[i] = 0;
	// 	for ( j = 0; j < n; j++ )
	// 	out[i] += mat[i][j] * vec[j];
	// }

	printf("\n");
	// Printez rezutlat
	for(i=0;i<n;i++)
		printf("%5.2f ",vec[i]);
	printf("\n");

	return 0;
}
