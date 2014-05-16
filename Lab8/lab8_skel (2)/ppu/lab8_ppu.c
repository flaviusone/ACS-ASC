#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <libspe2.h>
#include <pthread.h>

extern spe_program_handle_t lab8_spu;

#define MAX_SPU_THREADS   16
#define ARR_SIZE	80000


#define spu_mfc_ceil16(value)   ((value +  15) &  ~15)



typedef struct {
	float* A;	// pointer to section in first input array
	float* B;	// pointer to section in second input array
	float* C;	// pointer to section of output array
	int dim;	// dimensiune transfer DMA
} pointers_t;

void *ppu_pthread_function(void *thread_arg) {

	spe_context_ptr_t ctx;
	pointers_t *arg = (pointers_t *) thread_arg;
    void *spe_argp, *spe_envp;

    /* Create SPE context */
	if ((ctx = spe_context_create (0, NULL)) == NULL) {
            perror ("Failed creating context");
            exit (1);
        }

	/* Load SPE program into context */
	if (spe_program_load (ctx, &lab8_spu)) {
            perror ("Failed loading program");
            exit (1);
        }

	/* TODO: monitorizati mesajele primite prin mailbox de la SPE-uri (vezi cerinta 6) */

	/* Run SPE context */
	unsigned int entry = SPE_DEFAULT_ENTRY;
	/* TODO: transferati prin argument adresa si dimensiunea transferului initial (vezi cerinta 2) */
    spe_argp = (void*)arg;
    printf("PPU %x\n", arg->A);
    spe_envp = (void*)sizeof(pointers_t*);
    spe_envp = (void*) spu_mfc_ceil16((unsigned int)spe_envp);
    spe_argp = (void*) spu_mfc_ceil16((unsigned int)spe_argp);
	if (spe_context_run(ctx, &entry, 0, spe_argp, spe_envp, NULL) < 0) {
		perror ("Failed running context");
		exit (1);
	}

	/* Destroy context */
	if (spe_context_destroy (ctx) != 0) {
            perror("Failed destroying context");
            exit (1);
        }

	pthread_exit(NULL);
}

int main()
{
    float A[ARR_SIZE] __attribute__ ((aligned(16)));
    float B[ARR_SIZE] __attribute__ ((aligned(16)));
    float C[ARR_SIZE] __attribute__ ((aligned(16)));

    int i, spu_threads;
    pthread_t threads[MAX_SPU_THREADS];
    pointers_t thread_arg[MAX_SPU_THREADS] __attribute__ ((aligned(16)));

    /*
     * Initialization
     */
    for (i=0;i<ARR_SIZE;i++) {
	A[i] = i;
	B[i] = ARR_SIZE - i;
	C[i] = 0;
    }

    /* 
     * Determine the number of SPE threads to create.
     */

    spu_threads = spe_cpu_info_get(SPE_COUNT_USABLE_SPES, -1);
    if (spu_threads > MAX_SPU_THREADS) spu_threads = MAX_SPU_THREADS;

    /* 
     * Create several SPE-threads to execute 'simple_spu'.
     */

    for(i = 0; i < spu_threads; i++) {

	/* TODO: populati cate o structura de tip pointers_t pentru fiecare thread in parte (vezi cerinta 2,3,5) */
	int no_elems = ARR_SIZE / spu_threads;
	int dim = no_elems * sizeof(float); // in acest caz 20000 bytes, pentru fiecare SPU
	thread_arg[i].A = A + i*no_elems;
	thread_arg[i].B = B + i*no_elems;
	thread_arg[i].C = C + i*no_elems;
	thread_arg[i].dim = dim;

        /* Create thread for each SPE context */
        if (pthread_create (&threads[i], NULL, &ppu_pthread_function, &thread_arg[i]))  {
            perror ("Failed creating thread");
            exit (1);
        }
    }

    /* Wait for SPU-thread to complete execution.  */
    for (i = 0; i < spu_threads; i++) {
        if (pthread_join (threads[i], NULL)) {
            perror("Failed pthread_join");
            exit (1);
        }
    }

    printf("\nThe program has successfully executed.\n");
    int pass = 1;
    for (i=0; i<ARR_SIZE;i++)
	if (C[i]!=ARR_SIZE) pass = 0;
    if (pass) printf("Result is correct.\n");
    else printf("RESULT IS INCORRECT!\n");
    return 0;
}
