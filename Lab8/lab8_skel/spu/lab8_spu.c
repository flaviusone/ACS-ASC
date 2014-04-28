#include <stdio.h>
#include <spu_intrinsics.h>
#include <spu_mfcio.h>

/* TODO: adaugati define de waitag, vezi exemple DMA */
#define waitag(t) mfc_write_tag_mask(1<<t); mfc_read_tag_status_all();
#define DATA_BUFFER_ENTRIES 64
#define DATA_BUFFER_SIZE DATA_BUFFER_ENTRIES * sizeof(float)

float data_a[DATA_BUFFER_ENTRIES] __attribute__ ((aligned (128)));
float data_b[DATA_BUFFER_ENTRIES] __attribute__ ((aligned (128)));

typedef struct {
	float* A;	// pointer to section in first input array
	float* B;	// pointer to section in second input array
	float* C;	// pointer to section of output array
	int dim;	// dimensiune transfer DMA
} pointers_t;

void print_vector(vector float *v, int length);

int main(unsigned long long speid, unsigned long long argp, unsigned long long envp)
{

	int i=0,j=0,dim, num;
	pointers_t p __attribute__ ((aligned(16)));
	uint32_t tag_id = mfc_tag_reserve();
	if (tag_id==MFC_TAG_INVALID){
		printf("SPU: ERROR can't allocate tag ID\n"); return -1;
	}

	/* TODO: obtineti prin DMA structura cu pointeri si dimensiunea transferurilor (vezi cerinta 2) */
    mfc_get((void*)(&p), argp, (uint32_t)envp, tag_id, 0, 0);
    waitag(tag_id);
    printf("SPU:%x\n", p.A);
	/* TODO: cititi in Local Store date din A si B (vezi cerinta 3) */
    mfc_get((void*)data_a, (uint32_t)p.A, DATA_BUFFER_SIZE, tag_id, 0, 0);
    waitag(tag_id);
   	mfc_get((void*)data_a, (uint32_t)p.A, DATA_BUFFER_SIZE, tag_id, 0, 0);
    mfc_read_tag_status_all();
    print_vector(data_a, DATA_BUFFER_ENTRIES);
    print_vector(data_b, DATA_BUFFER_ENTRIES);
	/* TODO: adunati element cu element A si B folosind operatii vectoriale (vezi cerinta 4) */

	/* TODO: scrieti in main storage date din C (vezi cerinta 5) */

	/* TODO: repetati pasii de mai sus de cate ori este nevoie pentru a acoperi toate elementele, si semnalati un ciclu prin mailbox (vezi cerinta 6) */

	return 0;
}

void print_vector(vector float *v, int length)
{
        int i;
        for (i = 0; i < length; i+=1)
           printf("%.2lf %.2lf %.2lf %.2lf ", v[i][0], v[i][1], v[i][2], v[i][3]);
        printf("\n");
}


