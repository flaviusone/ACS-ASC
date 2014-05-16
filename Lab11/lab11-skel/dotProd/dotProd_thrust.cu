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
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/reduce.h>
#include <thrust/transform.h>
// For the CUDA runtime routines (prefixed with "cuda_")
#include <cuda_runtime.h>
/**
 * Host main routine
 */


float make_rand()
{
	return rand()/(float)RAND_MAX;
}

int main(int argc, char **argv)
{

    int numElements = 1024;
    printf("[Vector dot product of %d elements]\n", numElements);

    thrust::host_vector<float> h_A(numElements);
    thrust::host_vector<float> h_B(numElements);

    srand(time(NULL));
    thrust::generate(h_A.begin(), h_A.end(), make_rand);
    thrust::generate(h_B.begin(), h_B.end(), make_rand);

    //TODO: delarați și inițializați vectorii de pe device d_A, d_B
    thrust::device_vector<float> d_A = h_A;
    thrust::device_vector<float> d_B = h_B;
    //TODO: delarați vectoruld e pe device d_C
    thrust::device_vector<float> d_C(numElements);
    //TODO: calculați produsul element cu element C[i] = A[i] * B[i] folosind transformări
    thrust::transform(d_A.begin(), d_A.end(), d_B.begin(), d_C.begin(), thrust::multiplies<float>() );
    //TODO: calculați suma elementelor lui C în dotProd_d folosind reduceri
    float dotProd_d = thrust::reduce(d_C.begin(), d_C.end(), (float) 0, thrust::plus<float>());

    // Verify that the result vector is correct
    float result = 0;
    for (int i = 0; i < numElements; ++i)
    {
	result += (h_A[i]*h_B[i]);
    }

    printf("CPU = %f\n", result);
    printf("GPU = %f\n", dotProd_d);

    if (fabs(result - dotProd_d) > 1e-2)
    {
	fprintf(stderr, "Result verification failed!\n");
    }

    printf("Done\n");
    return 0;
}

