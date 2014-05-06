// Alex O., May 1st, 2011
// main function for matmul program

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matmul.h"

int main()
{
	int i,j,k;
	Matrix A, B, C, Cref;
	int correct = 1;

	// Allocate and initialize A, B	and C
	A.width = W;
	A.height = H;
	A.elements = (float*)malloc(A.width*A.height*sizeof(float));
	for (i=0;i<A.height;i++)
		for (j=0;j<A.width;j++)
			A.elements[i*A.width+j] = i*3+j;
	B.width = W;
	B.height = H;
	B.elements = (float*)malloc(B.width*B.height*sizeof(float));
	for (i=0;i<B.height;i++)
		for (j=0;j<B.width;j++)
			B.elements[i*B.width+j] = i*2+j;
	C.width = W;
	C.height = H;
	C.elements = (float*)malloc(C.width*C.height*sizeof(float));
	for (i=0;i<A.height;i++)
		for (j=0;j<A.width;j++)
			C.elements[i*C.width+j] = 0;

	printf("Computing multiplication on CPU\n");
	Cref.width = W;
	Cref.height = H;
	Cref.elements = (float*)malloc(Cref.width*Cref.height*sizeof(float));
	for (i=0;i<A.height;i++)
		for (j=0;j<B.width;j++) {
			Cref.elements[i*Cref.width+j]=0;
			for (k=0;k<A.height;k++)
				Cref.elements[i*Cref.width+j] += A.elements[i*A.width+k]*B.elements[k*B.width+j];
		}
	
	printf("Computing multiplication on GPU\n");
	MatMul(A,B,C);			
	printf("Checking result\n");
	for (i=0;i<C.height;i++)
		for(j=0;j<C.width;j++)
		{
#if DEBUG
			printf("%f %f\n", C.elements[i*C.width+j]+0, Cref.elements[i*Cref.width+j]);
#endif
			if (isnan(C.elements[i*C.width+j]) || abs(C.elements[i*C.width+j]-Cref.elements[i*Cref.width+j])>0.01)
				correct = 0;
		}

	if (correct)
		printf("PASSED\n");
	else
		printf("FAILED\n");

	free(A.elements);
	free(B.elements);
	free(C.elements);
	return 0;

}
