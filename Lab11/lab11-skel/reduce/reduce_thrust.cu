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

template <typename T>
struct square
{
	//TODO: definiți opearatorul care calculează pătratul unei valori
    __host__ __device__ T operator() (const T &a) const {
        return a * a;
    }
};

/**
 * Host main routine
 */


float make_rand()
{
	return rand()/(float)RAND_MAX;
}

int main(int argc, char **argv)
{

    // Print the vector length to be used, and compute its size
    int numElements = 1024;
    printf("[Reduce vector of %d elements]\n", numElements);

    thrust::host_vector<float> h_A(numElements);

    square<float> unary_op;
    thrust::plus<float> plus;
    float init = 0.0;

    float res_d = 0.0, res_h = 0.0;

    srand(time(NULL));
    thrust::generate(h_A.begin(), h_A.end(), make_rand);

    //TODO: declarați și inițializați vectorul d_A de pe device
    thrust::device_vector<float> d_A = h_A;
    //TODO: declarați vectorul C de lungime numElements pe device
    thrust::device_vector<float> d_C(numElements);
    //TODO: calculați folosind transform_reduce suma pătratelor elementelor lui d_A în res_d
    res_d = thrust::transform_reduce(d_A.begin(), d_A.end(), square<float>(), init, plus);

    //TODO: calculați folosind transform_reduce suma pătratelor elementelor lui h_A în res_h
    res_h = thrust::transform_reduce(h_A.begin(), h_A.end(), square<float>(), init, plus);


    printf("CPU = %f\n", res_h);
    printf("GPU = %f\n", res_d);

    if (fabs(res_h - res_d) > 1e-2)
    {
	fprintf(stderr, "Result verification failed!\n");
    }

    printf("Done\n");
    return 0;
}

