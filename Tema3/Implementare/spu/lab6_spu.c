/*
 * Computer System Architecture - Lab 6
 * SPU code
 */

#include <stdio.h>
#include <spu_intrinsics.h>


#define N  16
float A[N] __attribute__ ((aligned(16))) = {2, 4, 8, 10,
						12, 14, 16, 18,
						4, 6, 6, 4,
						10, 12, 12, 10};
float B[N] __attribute__ ((aligned(16)));
float C[N] __attribute__ ((aligned(16))) = { 	1.0, 2.0, 4.0, 5.0,
						6.0, 7.0, 8.0, 9.0,
						2.0, 3.0, 3.0, 2.0,
						5.0, 6.0, 6.0, 5.0};


/*
	Print an array of vector float variables with the given length.
	!!! Don't use for scalar arrays !!!
*/
void print_vector_float(vector float *v, int length)
{
        int i;
        for (i = 0; i < length; i+=1)
           printf("%.2lf %.2lf %.2lf %.2lf ", v[i][0], v[i][1], v[i][2], v[i][3]);
        printf("\n");
}
/*
	Print an array of vector unsigned int variables with the given length.
	!!! Don't use for scalar arrays !!!
*/
void print_vector_uint(vector unsigned int *v, int length)
{
        int i;
        for (i = 0; i < length; i+=1)
           printf("%u %u %u %u ", v[i][0], v[i][1], v[i][2], v[i][3]);
        printf("\n");
}

void task5()
{

	// TODO Task 5


	vector float scale_factor = spu_splats(0.5f);
	// 'transform' scalar arrays A, B, C to vector arrays: a, b, c
	vector float * a = (vector float *) A;
	vector float * b = (vector float *) B;
	vector float * c = (vector float *) C;

	int i;
	int n = N/4;

        // b = a * scale_factor
	for(i = 0; i < n; i++)
		b[i] = a[i] * scale_factor;

	// compare b and c
	vector unsigned int cmp_res[n];
	for(i = 0; i < n; i++)
		cmp_res[i] = spu_cmpeq(b[i],c[i]);

	print_vector_uint (cmp_res, n);
	print_vector_float(b, n);
	print_vector_float(c, n);

}
int main(unsigned long long speid,
		unsigned long long argp,
		unsigned long long envp)
{

	printf("Hello World! from Cell no %lld, id (0x%llx)\n", argp, speid);

	// TODO Task 4 - print the spe index received as argument

	// task5();

	return 0;
}




