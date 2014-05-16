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
reduce(const float *A, float *C, float *res, int numElements)
{
    int j;
   //TODO: calculați indexul threadului
   int no_thread = threadIdx.x + blockDim.x * blockIdx.x;
   //TODO: fiecare thread calculează în C pătratul elementului corespunzător din A
   if (no_thread < numElements) {
        C[no_thread] = A[no_thread] * A[no_thread];
   }
   __syncthreads();
   //TODO: unul dintre threaduri face suma și o returnează în res
    if (no_thread == 0) {
        float sum = 0;
        for (j = 0; j < numElements; ++j)
            sum += C[j];
        res[0] = sum;
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
    printf("[Reduce vector of %d elements]\n", numElements);

    // Allocați vectorii h_A, h_B, h_C de pe host si pointerul ce reține rezultatul h_res
    float *h_A, *h_C, *h_res;
    h_A = (float *) malloc(size);
    h_C = (float *) malloc(size);
    h_res = (float *) malloc (sizeof(float));
    //TODO: inițialiați vectorul h_A de pe host
    srand(time(NULL));
    for (int i = 0; i < numElements; i++) {
        h_A[i] = rand() / 100;
    }
    float *d_A, *d_C, *d_res;
    //TODO: alocați spațiu pe device pt cei 2 vectori A și C și pentru pointerul d_res ce reține rezultatul
    cudaMalloc( (void**) &d_A, size);
    cudaMalloc( (void**) &d_C, size);
    cudaMalloc( (void**) &d_res, sizeof(float));
    //TODO: copiați din h_A în d_A
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);

    // Launch the Vector Dot Product CUDA Kernel
    int threadsPerBlock = 32;
    int blocksPerGrid =(numElements + threadsPerBlock - 1) / threadsPerBlock;
    printf("CUDA kernel launch with %d blocks of %d threads\n", blocksPerGrid, threadsPerBlock);
    reduce<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_C, d_res, numElements);

    //TODO: copiați rezultatul de pe device (d_res) pe host (h_res)
    cudaMemcpy(h_res, d_res, sizeof(float), cudaMemcpyDeviceToHost);
    // Verify that the result vector is correct
    float result = 0;
    for (int i = 0; i < numElements; ++i)
    {
	result += (h_A[i]*h_A[i]);
    }

    printf("CPU = %f\n", result);
    printf("GPU = %f\n", h_res[0]);

    if (fabs(result - h_res[0]) > 1e-5)
    {
	fprintf(stderr, "Result verification failed!\n");
    }


    // TODO: eliberați memoria ocupată de d_A, d_C și d_res pe device
    cudaFree(d_A);
    cudaFree(d_C);
    cudaFree(d_res);

    free(h_A);
    free(h_C);
    free(h_res);

    printf("Done\n");
    return 0;
}

