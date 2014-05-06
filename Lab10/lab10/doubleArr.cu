#include <stdlib.h>
#include <stdio.h>

#define MAX_ERR 0.0001
#define N 10000
#define BS 2

void doubleArrayOnHost (float* s, float* d, int size)
{
	int i;
	for ( i=0; i<size; i++) d[i] = 2*s[i];
}

__global__ void doubleArrayOnDevice(float* temp, int size)
{
	int index = blockIdx.x*blockDim.x + threadIdx.x;
	if (index<N) temp[index] = 2*temp[index];
}

int main()
{
	int i=0;
	int correct = 1;
	float* p1 = (float*) malloc( N *sizeof(float) );	// original data source
	float* p2 = (float*) malloc( N *sizeof(float) ); 	// holds result computed by CPU
	float* pgpu = (float*) malloc( N *sizeof(float) ); 	// holds result obtained from GPU
	float* pd = 0; 						// pointer to device memory

	// initialize array values
	srand((unsigned int)time((time_t *)NULL));
	for ( i=0; i<N; i++) p1[i] = rand()/10;
	// compute the result on the host
	doubleArrayOnHost(p1, p2, N);

	// compute the result on device
	cudaMalloc( (void**) &pd, N *sizeof(float)); 	// the second parameter is the addres of the pointer, not the pointer itself
	cudaMemcpy( pd, p1, N*sizeof(float), cudaMemcpyHostToDevice);
	int bs = BS; // runtime chosen value for blocksize! 

	// if N is not a multiple of bs, 
	// an extra block is needed
	int nb = N / bs + (N%bs == 0 ? 0:1); 
	doubleArrayOnDevice <<< nb, bs >>> (pd, N);

	cudaMemcpy( pgpu, pd, N*sizeof(float), cudaMemcpyDeviceToHost);
	cudaFree(pd);

	// check consistency
	for ( i=0; i<N; i++)
	{
		if (abs(p2[i] - pgpu[i])>MAX_ERR) correct = 0;
	}
	if (correct) {
		printf("PASSED\n");
	} else {
		printf("FAILED\n");
	}
	free(p1); free(p2); free(pgpu);
}
