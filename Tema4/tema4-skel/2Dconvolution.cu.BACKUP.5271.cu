/**
 * Tema 4 ASC - 2D Convolution
 *
 * Copyright (C) 2014, Flavius Tirnacop 331CA <flavius.tirnacop@cti.pub.ro>
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 */

/*
 * Copyright 1993-2006 NVIDIA Corporation.  All rights reserved.
 *
 * NOTICE TO USER:
 *
 * This source code is subject to NVIDIA ownership rights under U.S. and
 * international Copyright laws.
 *
 * This software and the information contained herein is PROPRIETARY and
 * CONFIDENTIAL to NVIDIA and is being provided under the terms and
 * conditions of a Non-Disclosure Agreement.  Any reproduction or
 * disclosure to any third party without the express written consent of
 * NVIDIA is prohibited.
 *
 * NVIDIA MAKES NO REPRESENTATION ABOUT THE SUITABILITY OF THIS SOURCE
 * CODE FOR ANY PURPOSE.  IT IS PROVIDED "AS IS" WITHOUT EXPRESS OR
 * IMPLIED WARRANTY OF ANY KIND.  NVIDIA DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOURCE CODE, INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OR PERFORMANCE OF THIS SOURCE CODE.
 *
 * U.S. Government End Users.  This source code is a "commercial item" as
 * that term is defined at 48 C.F.R. 2.101 (OCT 1995), consisting  of
 * "commercial computer software" and "commercial computer software
 * documentation" as such terms are used in 48 C.F.R. 12.212 (SEPT 1995)
 * and is provided to the U.S. Government only as a commercial end item.
 * Consistent with 48 C.F.R.12.212 and 48 C.F.R. 227.7202-1 through
 * 227.7202-4 (JUNE 1995), all U.S. Government End Users acquire the
 * source code with only those rights set forth herein.
 */

// includes, system
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <helper_cuda.h>
#include <helper_timer.h>
#include <helper_functions.h>
#include <helper_math.h>

// includes, project
#include "2Dconvolution.h"


////////////////////////////////////////////////////////////////////////////////
// declarations, forward

extern "C"
void computeGold(float*, const float*, const float*, unsigned int, unsigned int);

Matrix AllocateDeviceMatrix(int width, int height);
Matrix AllocateMatrix(int width, int height);
void FreeDeviceMatrix(Matrix* M);
void FreeMatrix(Matrix* M);

void ConvolutionOnDevice(const Matrix M, const Matrix N, Matrix P);
void ConvolutionOnDeviceShared(const Matrix M, const Matrix N, Matrix P);

////////////////////////////////////////////////////////////////////////////////
// Înmulțirea fără memorie partajată
////////////////////////////////////////////////////////////////////////////////
__global__ void ConvolutionKernel(Matrix M, Matrix N, Matrix P)
{

    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    float sum=0;
    int m,n;

    if ((row >= N.height) || (col >= N.width) || (row < 0) || (col < 0)) return;

    for (m = 0 ; m < 5 ; m++)
        for (n=0 ; n < 5 ; n++) {
                if((row+m-2 >= 0) && (row+m-2 < N.height) && (col+n-2 >= 0) && (col+n-2 < N.width))
                    sum += M.elements[m*M.width+n] * N.elements[(row+m-2) * N.width+(col+n-2)];
        }
    P.elements[row*P.width+col] = sum;

}


////////////////////////////////////////////////////////////////////////////////
// Înmulțirea cu memorie partajată
////////////////////////////////////////////////////////////////////////////////
__global__ void ConvolutionKernelShared(Matrix M, Matrix N, Matrix P)
{

<<<<<<< HEAD
    __shared__ float Ms[5][5];
    __shared__ float Ns[BLOCK_SIZE+4][BLOCK_SIZE+4];
    int m,n;
    float sum=0;

    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    // Block row and column
    int blockRow = blockIdx.y;
    int blockCol = blockIdx.x;
    int threadRow = threadIdx.y;
    int threadCol = threadIdx.x;

    if ((row >= N.height) || (col >= N.width) || (row < 0) || (col < 0)) return;

    // Pas 1 copiere matrice Kernel local
    if(threadIdx.x < 5 && threadIdx.y < 5){
        Ms[threadCol][threadRow] = M.elements[threadRow*5+threadCol];
    }

    // Pas 2 copiere centru matrice local
    // Fiecare thread copiaza block-ul interior in matricea shared
    Ns[threadRow+2][threadCol+2] = N.elements[N.width * BLOCK_SIZE * blockRow + N.width * threadRow + threadCol];

    // Pas 3 copiere bordarea de sus a matricei
    if(threadRow == 0)
        Ns[threadRow][threadCol+2] = N.elements[N.width * (BLOCK_SIZE-2) * blockRow + N.width * threadRow + threadCol];
    if(threadRow == 1)
        Ns[threadRow+1][threadCol+2] = N.elements[N.width * (BLOCK_SIZE-1) * blockRow + N.width * threadRow + threadCol];

    // Pas 4 copiere bordarea de jos a matricei
    if(threadRow == 14)
        Ns[threadRow+2][threadCol+2] = N.elements[N.width * (BLOCK_SIZE+2) * blockRow + N.width * threadRow + threadCol];
    if(threadRow == 15)
        Ns[threadRow+1][threadCol+2] = N.elements[N.width * (BLOCK_SIZE+1) * blockRow + N.width * threadRow + threadCol];

    // Pas 5 copiere bordarea din stanga matricei
    if(threadCol == 0)
        Ns[threadRow+2][threadCol] = N.elements[N.width * BLOCK_SIZE * blockRow + N.width * threadRow + threadCol-2];
    if(threadCol == 1)
        Ns[threadRow+2][threadCol+1] = N.elements[N.width * BLOCK_SIZE * blockRow + N.width * threadRow + threadCol-1];


    // Pas 6 copiere bordarea din dreapta matricei
    if(threadCol == 14)
        Ns[threadRow+2][threadCol+2] = N.elements[N.width * BLOCK_SIZE * blockRow + N.width * threadRow + threadCol+2];
    if(threadCol == 15)
        Ns[threadRow+2][threadCol+1] = N.elements[N.width * BLOCK_SIZE * blockRow + N.width * threadRow + threadCol+2];


    // Pas 7 copiere cele 16 colturi
    if (threadRow == 0 && threadCol == 0){
        Ns[0][0] = N.elements[N.width * (BLOCK_SIZE-2) * blockRow + N.width * threadRow + threadCol - 2];
        Ns[0][1] = N.elements[N.width * (BLOCK_SIZE-2) * blockRow + N.width * threadRow + threadCol - 1];
        Ns[1][0] = N.elements[N.width * (BLOCK_SIZE-1) * blockRow + N.width * threadRow + threadCol - 2];
        Ns[1][1] = N.elements[N.width * (BLOCK_SIZE-1) * blockRow + N.width * threadRow + threadCol - 1];
    }

    if (threadRow == 0 && threadCol == 14){
        Ns[0][18] = N.elements[N.width * (BLOCK_SIZE-2) * blockRow + N.width * threadRow + threadCol + 1];
        Ns[0][19] = N.elements[N.width * (BLOCK_SIZE-2) * blockRow + N.width * threadRow + threadCol + 2];
        Ns[1][18] = N.elements[N.width * (BLOCK_SIZE-1) * blockRow + N.width * threadRow + threadCol + 1];
        Ns[1][19] = N.elements[N.width * (BLOCK_SIZE-1) * blockRow + N.width * threadRow + threadCol + 2];
    }

    if (threadRow == 14 && threadCol == 0){
        Ns[18][0] = N.elements[N.width * (BLOCK_SIZE+1) * blockRow + N.width * threadRow + threadCol - 2];
        Ns[18][1] = N.elements[N.width * (BLOCK_SIZE+1) * blockRow + N.width * threadRow + threadCol - 1];
        Ns[19][0] = N.elements[N.width * (BLOCK_SIZE+2) * blockRow + N.width * threadRow + threadCol - 2];
        Ns[19][1] = N.elements[N.width * (BLOCK_SIZE+2) * blockRow + N.width * threadRow + threadCol - 1];
    }

    if (threadRow == 14 && threadCol == 14){
        Ns[18][18] = N.elements[N.width * (BLOCK_SIZE+1) * blockRow + N.width * threadRow + threadCol + 1];
        Ns[18][19] = N.elements[N.width * (BLOCK_SIZE+1) * blockRow + N.width * threadRow + threadCol + 2];
        Ns[19][18] = N.elements[N.width * (BLOCK_SIZE+2) * blockRow + N.width * threadRow + threadCol + 1];
        Ns[19][19] = N.elements[N.width * (BLOCK_SIZE+2) * blockRow + N.width * threadRow + threadCol + 2];
    }

    // Synchronize to make sure that the preceding computation is done before
    __syncthreads();

    for (m = 0 ; m < 5 ; m++)
        for (n=0 ; n < 5 ; n++){
            //sum += M.elements[m*M.width+n] * N.elements[(row+m-2) * N.width+(col+n-2)];
            sum += M.elements[m*M.width+n] * Ns[threadRow+m-2][threadCol+n-2];
        }


    P.elements[row*P.width+col] = sum;

=======
    //TODO: calculul rezultatului convoluției
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    P.elements[row*P.width+col] = 0;
>>>>>>> 657513d0c187e9038dc054344f2daf073b0ff4a3
    return;
}

////////////////////////////////////////////////////////////////////////////////
// Returnează 1 dacă matricele sunt ~ egale
////////////////////////////////////////////////////////////////////////////////
int CompareMatrices(Matrix A, Matrix B)
{
    int i;
    if(A.width != B.width || A.height != B.height || A.pitch != B.pitch)
        return 0;
    int size = A.width * A.height;
    for(i = 0; i < size; i++){
        //printf("%f %f\n", A.elements[i] , B.elements[i]);
        if(fabs(A.elements[i] - B.elements[i]) > MAX_ERR)
            return 0;
    }
    return 1;
}
void GenerateRandomMatrix(Matrix m)
{
    int i;
    int size = m.width * m.height;

    srand(time(NULL));

    for(i = 0; i < size; i++)
        m.elements[i] = rand() / (float)RAND_MAX;
}

////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
    int width = 0, height = 0;
    FILE *f, *out;
    if(argc < 2)
    {
        printf("Argumente prea puține, trimiteți id-ul testului care trebuie rulat\n");
        return 0;
    }
    char name[100];
    sprintf(name, "./tests/test_%s.txt", argv[1]);
    f = fopen(name, "r");
    out = fopen("out.txt", "a");
    fscanf(f, "%d%d", &width, &height);
    Matrix M;//kernel de pe host
    Matrix N;//matrice inițială de pe host
    Matrix P;//rezultat fără memorie partajată calculat pe GPU
    Matrix PS;//rezultatul cu memorie partajată calculat pe GPU

    M = AllocateMatrix(KERNEL_SIZE, KERNEL_SIZE);
    N = AllocateMatrix(width, height);
    P = AllocateMatrix(width, height);
    PS = AllocateMatrix(width, height);

    GenerateRandomMatrix(M);
    GenerateRandomMatrix(N);


    // M * N pe device
    ConvolutionOnDevice(M, N, P);

    // M * N pe device cu memorie partajată
    ConvolutionOnDeviceShared(M, N, PS);

    // calculează rezultatul pe CPU pentru comparație
    Matrix reference = AllocateMatrix(P.width, P.height);
    computeGold(reference.elements, M.elements, N.elements, N.height, N.width);

    // verifică dacă rezultatul obținut pe device este cel așteptat
    int res = CompareMatrices(reference, P);
    printf("Test global %s\n", (1 == res) ? "PASSED" : "FAILED");
    fprintf(out, "Test global %s %s\n", argv[1], (1 == res) ? "PASSED" : "FAILED");

    // verifică dacă rezultatul obținut pe device cu memorie partajată este cel așteptat
    int ress = CompareMatrices(reference, PS);
    printf("Test shared %s\n", (1 == ress) ? "PASSED" : "FAILED");
    fprintf(out, "Test shared %s %s\n", argv[1], (1 == ress) ? "PASSED" : "FAILED");

    // Free matrices
    FreeMatrix(&M);
    FreeMatrix(&N);
    FreeMatrix(&P);
    FreeMatrix(&PS);

    fclose(f);
    fclose(out);
    return 0;
}


////////////////////////////////////////////////////////////////////////////////
//! Run a simple test for CUDA
////////////////////////////////////////////////////////////////////////////////
void ConvolutionOnDevice(const Matrix M, const Matrix N, Matrix P)
{
    size_t size;
    Matrix Md, Nd, Pd; //matricele corespunzătoare de pe device

    //pentru măsurarea timpului de execuție în kernel
    StopWatchInterface *kernelTime = NULL;
    sdkCreateTimer(&kernelTime);
    sdkResetTimer(&kernelTime);
    //TODO: alocați matricele de pe device
    Md = AllocateDeviceMatrix(M.width, M.height);
    Nd = AllocateDeviceMatrix(N.width, N.height);
    Pd = AllocateDeviceMatrix(P.width, P.height);

    //TODO: copiați datele de pe host (M, N) pe device (MD, Nd)
    size = M.width * M.height * sizeof(float);
    cudaMemcpy(Md.elements, M.elements, size, cudaMemcpyHostToDevice);
    size = N.width * N.height * sizeof(float);
    cudaMemcpy(Nd.elements, N.elements, size, cudaMemcpyHostToDevice);

    //TODO: setați configurația de rulare a kernelului
    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
    dim3 dimGrid((N.width-1) / dimBlock.x + 1, (N.height-1) / dimBlock.y + 1);

    sdkStartTimer(&kernelTime);
    //TODO: lansați în execuție kernelul
    ConvolutionKernel<<<dimGrid, dimBlock>>>(Md, Nd, Pd);

    cudaThreadSynchronize();
    sdkStopTimer(&kernelTime);
    printf ("Timp execuție kernel: %f ms\n", sdkGetTimerValue(&kernelTime));
    //TODO: copiaţi rezultatul pe host
    size = P.width * P.height * sizeof(float);
    cudaMemcpy(P.elements, Pd.elements, size, cudaMemcpyDeviceToHost);

    //TODO: eliberați memoria matricelor de pe device\
    FreeDeviceMatrix(&Md);
    FreeDeviceMatrix(&Nd);
    FreeDeviceMatrix(&Pd);
}


void ConvolutionOnDeviceShared(const Matrix M, const Matrix N, Matrix P)
{
    size_t size;
    Matrix Md, Nd, Pd; //matricele corespunzătoare de pe device

    //pentru măsurarea timpului de execuție în kernel
    StopWatchInterface *kernelTime = NULL;
    sdkCreateTimer(&kernelTime);
    sdkResetTimer(&kernelTime);
    //TODO: alocați matricele de pe device
    Md = AllocateDeviceMatrix(M.width, M.height);
    Nd = AllocateDeviceMatrix(N.width, N.height);
    Pd = AllocateDeviceMatrix(P.width, P.height);

    //TODO: copiați datele de pe host (M, N) pe device (MD, Nd)
    size = M.width * M.height * sizeof(float);
    cudaMemcpy(Md.elements, M.elements, size, cudaMemcpyHostToDevice);
    size = N.width * N.height * sizeof(float);
    cudaMemcpy(Nd.elements, N.elements, size, cudaMemcpyHostToDevice);

    //TODO: setați configurația de rulare a kernelului
    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
    dim3 dimGrid((N.width-1) / dimBlock.x + 1, (N.height-1) / dimBlock.y + 1);

    sdkStartTimer(&kernelTime);
    //TODO: lansați în execuție kernelul
    ConvolutionKernelShared<<<dimGrid, dimBlock>>>(Md, Nd, Pd);
    cudaThreadSynchronize();
    sdkStopTimer(&kernelTime);
    printf ("Timp execuție kernel cu memorie partajată: %f ms\n", sdkGetTimerValue(&kernelTime));
    //TODO: copiaţi rezultatul pe host
    size = P.width * P.height * sizeof(float);
    cudaMemcpy(P.elements, Pd.elements, size, cudaMemcpyDeviceToHost);

    //TODO: eliberați memoria matricelor de pe device
    FreeDeviceMatrix(&Md);
    FreeDeviceMatrix(&Nd);
    FreeDeviceMatrix(&Pd);
}


// Alocă o matrice de dimensiune height*width pe device
Matrix AllocateDeviceMatrix(int width, int height)
{
    Matrix m;
    //TODO: alocați matricea și setați width, pitch și height
    m.width = m.pitch = width;
    m.height = height;
    int size = m.width * m.height * sizeof(float);
    cudaMalloc((void **) &m.elements, size);
    return m;
}

// Alocă matrice pe host de dimensiune height*width
Matrix AllocateMatrix(int width, int height)
{
    Matrix M;
    M.width = M.pitch = width;
    M.height = height;
    int size = M.width * M.height;
    M.elements = (float*) malloc(size*sizeof(float));
    return M;
}

// Eliberează o matrice de pe device
void FreeDeviceMatrix(Matrix* M)
{
    cudaFree(M->elements);
    M->elements = NULL;
}

// Eliberează o matrice de pe host
void FreeMatrix(Matrix* M)
{
    free(M->elements);
    M->elements = NULL;
}
