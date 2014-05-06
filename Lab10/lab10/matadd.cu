#include <stdlib.h>
#include <stdio.h>

#define MAX_ERR 0.0001
#define N 10000
#define BS 2

void matAddOnHost(float *a, float *b, float *c){
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            c[i * N + j] = a[i * N + j] + b[i * N + j];
        }
    }
}

__global__ void MatAdd(float *A, float *B, float *C) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    if(i < N && j < N) {
        C[i*N+j] = A[i*N+j] + B[i*N+j];
    }
}

int main() {
    int i = 0;
    float a[N*N], b[N*N], c[N*N];
    float *A, *B, *C;
    float *pgpu = (float*) malloc(N*N*sizeof(float));

    /* Umplem matricea cu elemente random*/
    srand((unsigned int) time((time_t *)NULL));
    for(i = 0; i < N * N; i++) {
        a[i] = rand() % 100;
        b[i] = rand() % 100;
    }
    /* Adunam pe host */
    matAddOnHost(a, b, c);


    /* Alocam spatiu pt matrice*/
    cudaMalloc((void**) &A, N * N * sizeof(float));
    cudaMalloc((void**) &B, N * N * sizeof(float));
    cudaMalloc((void**) &C, N * N * sizeof(float));

    /* Le umplem */
    cudaMemcpy(A, a, N*N*sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(B, b, N*N*sizeof(float), cudaMemcpyHostToDevice);


    /* Magic happens*/
    dim3 dimBlock(BS, BS);
    dim3 dimGrid((N + dimBlock.x - 1) / dimBlock.x, (N + dimBlock.y - 1) / dimBlock.y);

    MatAdd<<<dimGrid, dimBlock>>>(A, B, C);

    /* Copy result to host*/
    cudaMemcpy(pgpu, C, N*N*sizeof(float), cudaMemcpyDeviceToHost);

    /* Free space */
    cudaFree(A);
    cudaFree(B);
    cudaFree(C);

    /* Check if passed*/
    int correct = 1;
    for(i = 0; i < N*N; i++) {
        if (abs(c[i] - pgpu[i])>MAX_ERR) correct = 0;
    }
    if(correct) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    free(pgpu);
}
