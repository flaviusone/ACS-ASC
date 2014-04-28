#include <stdio.h>
#include <spu_intrinsics.h>
#include <spu_mfcio.h>

#define waitag(t) mfc_write_tag_mask(1<<t); mfc_read_tag_status_all();

typedef struct {
	float* A;	// pointer to section in first input array
	float* B;	// pointer to section in second input array
	float* C;	// pointer to section of output array
	int dim;	// dimensiune transfer DMA
} pointers_t;

void print_vector(vector float *v, int length);

int main(unsigned long long speid, unsigned long long argp, unsigned long long envp)
{

	int i;				// numarul iteratiei, in cazul nostru vor fi 10 iteratii
	int j;				// indice folosit in parcurgerea datelor pentru a calcula sumele
	int dim=10000;		// dimensiunea unui transfer, in cazul nostru vom lua cate 10000 de bytes din cei specificati de p.dim
	int buf, nxt_buf;   // indexul bufferului, 0 sau 1
	pointers_t p __attribute__ ((aligned(16)));
	uint32_t tag_id[2];	// de data asta avem nevoie de doua tag_id
	tag_id[0] = mfc_tag_reserve();
	if (tag_id[0]==MFC_TAG_INVALID){
		printf("SPU: ERROR can't allocate tag ID\n"); return -1;
	}
	tag_id[1] = mfc_tag_reserve();
	if (tag_id[1]==MFC_TAG_INVALID){
		printf("SPU: ERROR can't allocate tag ID\n"); return -1;
	}

	/* transferul initial, cu structura de pointeri */
	mfc_get((void*)&p, argp, (int) envp, tag_id[0], 0, 0);
	waitag(tag_id[0]);

	float A[2][dim] __attribute__ ((aligned(16)));  //  ...[2]... = DOUBLE buffering :D
	float B[2][dim] __attribute__ ((aligned(16)));
	float C[2][dim] __attribute__ ((aligned(16)));

	// primul transfer de date, in afara buclei
	buf = 0;
	/* TODO(1): folositi mfc_get cu bariera pentru a lua primul set de date din A si B */
	mfc_getb((void*) (&(A[buf][0])), (uint32_t) p.A, (uint32_t) dim, tag_id[buf], 0, 0);
    mfc_getb((void*) (&(B[buf][0])), (uint32_t) p.B, (uint32_t) dim, tag_id[buf], 0, 0);
	i=1; // one transfer already running
	while (i<p.dim/dim) { // cat timp nu s-a terminat de luat tot

		printf("%llx: %d\n",speid,i);
		// Cer bufferul urmator de date de la PPU
		// Atentie la bariera!
		nxt_buf = buf^1;
		/* TODO(1): folositi mfc_get cu bariera pentru a lua urmatorul set de date din A si B */
   		mfc_getb((void*) (&(A[nxt_buf][0])), ((uint32_t) p.A) + i * dim, (uint32_t) dim, tag_id[nxt_buf], 0, 0);
        mfc_getb((void*) (&(B[nxt_buf][0])), ((uint32_t) p.B) + i * dim, (uint32_t) dim, tag_id[nxt_buf], 0, 0);

		// Astept bufferul precedent de date de la PPU
		waitag(tag_id[buf]);

		// Procesez bufferul precedent
		/* TODO(1): procesati datele folosind operatii vectoriale (adunare element cu element) */
		vector float* a = (vector float*) &(A[buf][0]);
		vector float* b = (vector float*) &(B[buf][0]);
		vector float* c = (vector float*) &(C[buf][0]);
        int count = dim / 16;
        for (j = 0; j < count; j++) {
            c[j] = a[j] + b[j];
        }
		// Trimit bufferul precedent la PPU
		/* TODO(1): folositi mfc_put pentru a trimite la PPU setul de date din C tocmai calculat */
        mfc_putb((void*) (&(C[buf][0])), ((uint32_t) p.C) + (i - 1) * dim, (uint32_t) dim, tag_id[buf], 0, 0);
		// Pregatim urmatoarea iteratie
		buf = nxt_buf;
		i++;

		// Confirm sfarsitul iteratiei
        printf("[SPU %x]: confirm etapa %d ... \n", (int) speid, i);
        /* TODO(2): trimiteti catre PPU prin mailbox-ul out_intre idul iteratiei care s-a incheiat*/
        while(spu_stat_out_mbox() <= 0);
        spu_write_out_mbox((uint32_t) i);
	}

	// Astept ultimul buffer de date de la PPU
	waitag(tag_id[buf]);

	// Procesez ultimul buffer
	/* TODO(1): procesati datele folosind operatii vectoriale (adunare element cu element) */
	vector float* a = (vector float*) &(A[buf][0]);
    vector float* b = (vector float*) &(B[buf][0]);
    vector float* c = (vector float*) &(C[buf][0]);

    int count = dim / 16;
    for (j = 0; j < count; j++) {
        c[j] = a[j] + b[j];
    }
	// Trimit ultimul buffer la PPU
	// Punem bariera pentru a ne asigura ca s-a trimis si ultimul rezultat inainte de a confirma statusul
	/* TODO(1): folositi mfc_put pentru a trimite la PPU ultimul set de date din C */
	mfc_putb((void*) (&(C[buf][0])), ((uint32_t) p.C) + (i - 1) * dim, (uint32_t) dim, tag_id[buf], 0, 0);
	waitag(tag_id[buf]);

	return 0;
}

void print_vector(vector float *v, int length)
{
        int i;
        for (i = 0; i < length; i+=1)
           printf("%.2lf %.2lf %.2lf %.2lf ", v[i][0], v[i][1], v[i][2], v[i][3]);
        printf("\n");
}

