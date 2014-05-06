// The width and the height of the matrices are assumed to be multiples of BLOCK_SIZE
#define W 1024
#define H 1024
#define DEBUG 0

// Matrices are stored in row-major order:
// M(row, col) = *(M.elements + row * M.width + col)
typedef struct {
  int width;
  int height;
  int stride;		// for shared memory case
  float* elements;
} Matrix;

// Forward declaration of the matrix multiplication function
void MatMul(const Matrix A, const Matrix B, Matrix C);
