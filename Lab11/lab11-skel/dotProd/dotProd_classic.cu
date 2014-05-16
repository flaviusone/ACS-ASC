/**
 * Copyright 1993-2012 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */

/**
 * Vector dot product: C = A + B.
 *
 * This sample is a very basic sample that implements
 * vector dot product. It is the same as the sample illustrating Chapter 2
 * of the programming guide with some additions like error checking.
 */

#include <stdio.h>

// For the CUDA runtime routines (prefixed with "cuda_")
#include <cuda_runtime.h>

/**
 * CUDA Kernel Device code
 *
 * Computes the vector dot product of A and B. The vectors have the same
 * number of elements numElements.
 */
__global__ void
vectorDot(const float *A, const float *B, float *C, float *dotProd, int numElements)
{
  int j;
  //TODO: calculați indexul threadului
  int thread_no = threadIdx.x + blockDim.x * blockIdx.x;
  //TODO: fiecare thread face calculul
  if (thread_no < numElements) {
        C[thread_no] = A[thread_no] * B[thread_no];
  }
   __syncthreads();
  //TODO: unul din threaduri calculează suma finală și o pune în dotProduct
  if (thread_no == 0) {
      float suma = 0;
      for ( j = 0; j < numElements; j++)
          suma += C[j];
      dotProd[0] = suma;
  }
}

/**
 * Host main routine
 */
int
main(int argc, char **argv)
{

    // Print the vector length to be used, and compute its size
    int numElements = 1024;
    size_t size = numElements * sizeof(float);

    printf("[Vector dot product of %d elements]\n", numElements);

    float *h_A, *h_B, *h_C, *dotProd_h;
    //TODO: alocați cei 3 vectori h_A, h_B, h_C pe host și dotProd_h care va reține suma finală
    h_A = (float *) malloc(size);
    h_B = (float *) malloc(size);
    h_C = (float *) malloc(size);

    //TODO: inițializați random cei doi vectori h_A si h_B
    srand(time(NULL));
    for (int i = 0; i < numElements; ++i) {
        h_A[i] = rand() / 10;
        h_B[i] = rand() / 10;
    }
    //TODO: Alocați vectorii d_A, d_B și d_C pe device și dotProd_d car va reține rezultatul sumei pe device
    float *d_A, *d_B, *d_C, *dotProd_d;

    cudaMalloc( (void**) &d_A, size);
    cudaMalloc( (void**) &d_B, size);
    cudaMalloc( (void**) &d_C, size);
    cudaMalloc( (void**) &dotProd_d, sizeof(float));
    dotProd_h = (float *) malloc (sizeof (float));
   //TODO: Copiați datele în d_A și d_B pe device din h_A și h_B
    cudaMemcpy(d_A, h_A, numElements * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, numElements * sizeof(float), cudaMemcpyHostToDevice);
    int threadsPerBlock = 32;
    int blocksPerGrid =(numElements + threadsPerBlock - 1) / threadsPerBlock;
    printf("CUDA kernel launch with %d blocks of %d threads\n", blocksPerGrid, threadsPerBlock);
    vectorDot<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, dotProd_d, numElements);

    //TODO: copiați datele de pe device pe host
    cudaMemcpy(dotProd_h, dotProd_d, sizeof(float), cudaMemcpyDeviceToHost);


    // Verify that the result vector is correct
    float result = 0;
    for (int i = 0; i < numElements; ++i)
    {
	result += (h_A[i]*h_B[i]);
    }

    printf("CPU = %f\n", result);
    printf("GPU = %f\n", dotProd_h[0]);

    if (fabs(result - dotProd_h[0]) > 1e-5)
    {
	fprintf(stderr, "Result verification failed!\n");
    }


    // Free device global memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    cudaFree(dotProd_d);


    // Free host memory
    free(h_A);
    free(h_B);
    free(h_C);
    free(dotProd_h);

    printf("Done\n");
    return 0;
}

