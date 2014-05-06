#include "matmul.h"
 
// Thread block size
#define BLOCK_SIZE 16

// Forward declaration of the matrix multiplication kernel
__global__ void MatMulKernel(const Matrix, const Matrix, Matrix);

// Matrix multiplication - Host code
void MatMul(const Matrix A, const Matrix B, Matrix C)
{
  // Load A and B to device memory
  Matrix d_A;
  d_A.width = A.width; 
  d_A.height = A.height;
  
  size_t size = A.width * A.height * sizeof(float);
  // TODO: alocati matricea d_A pe device si puneti in ea date din matricea A de pe host
  cudaMalloc((void **) &d_A.elements, size);
  cudaMemcpy(d_A.elements, A.elements, size, cudaMemcpyHostToDevice);

  Matrix d_B;

  d_B.width = B.width; 
  d_B.height = B.height;
  
  size = B.width * B.height * sizeof(float);
  // TODO: alocati matricea d_B pe device si puneti in ea date din matricea B de pe host
  cudaMalloc((void **) &d_B.elements, size);
  cudaMemcpy(d_B.elements, B.elements, size, cudaMemcpyHostToDevice);


  // Allocate C in device memory
  Matrix d_C;

  d_C.width = C.width; 
  d_C.height = C.height;
  
  size = C.width * C.height * sizeof(float);
  cudaMalloc((void**)&d_C.elements, size);

  // Invoke kernel
  dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
  dim3 dimGrid(B.width / dimBlock.x, A.height / dimBlock.y);

  MatMulKernel<<<dimGrid, dimBlock>>>(d_A, d_B, d_C);

  // Read C from device memory
  // TODO: copiati in matricea C de pe host rezultatul din matricea d_C de pe device
  cudaMemcpy(C.elements, d_C.elements, size, cudaMemcpyDeviceToHost);
  // Free device memory
  cudaFree(d_A.elements);
  cudaFree(d_B.elements);
  cudaFree(d_C.elements);
}

// Matrix multiplication kernel called by MatrixMul()
__global__ void MatMulKernel(Matrix A, Matrix B, Matrix C)
{
  // Each thread computes one element of C
  // by accumulating results into Cvalue
  float Cvalue = 0;
  int row = blockIdx.y * blockDim.y + threadIdx.y;
  int col = blockIdx.x * blockDim.x + threadIdx.x;

  for (int e = 0; e < A.width; ++e)
     Cvalue += A.elements[row * A.width + e] * B.elements[e * B.width + col];


  C.elements[row * C.width + col] = Cvalue;

}
