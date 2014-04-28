#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <libspe2.h>
#include <pthread.h>
#include <math.h>
#include <string.h>

#include "btc.h"


extern spe_program_handle_t lab6_spu;

#define MAX_SPU_THREADS   16

typedef struct {
	int cellno;
} thread_arg_t;

void *ppu_pthread_function(void *thread_arg) {

	spe_context_ptr_t ctx;
	thread_arg_t *arg = (thread_arg_t *) thread_arg;

	/* Create SPE context */

	if ((ctx = spe_context_create (0, NULL)) == NULL) {
		perror ("Failed creating context");
		exit (1);
	}

	/* Load SPE program into context */

	if (spe_program_load (ctx, &lab6_spu)) {
		perror ("Failed loading program");
		exit (1);
	}


	/* Run SPE context */

	unsigned int entry = SPE_DEFAULT_ENTRY;
	if (spe_context_run(ctx, &entry, 0, (void *) arg->cellno, NULL, NULL) < 0) {
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

int main(int argc, char* argv[])
{
	int i, spu_threads;
	pthread_t threads[MAX_SPU_THREADS];
	thread_arg_t thread_arg[MAX_SPU_THREADS];

	/*
	 * Determine the number of SPE threads to create.
	 */

	spu_threads = spe_cpu_info_get(SPE_COUNT_USABLE_SPES, -1);
	if (spu_threads > MAX_SPU_THREADS) spu_threads = MAX_SPU_THREADS;

	/*
	 * Create several SPE-threads to execute 'simple_spu'.
	 */

	for(i = 0; i < spu_threads; i++) {

		/* Create thread for each SPE context */
		thread_arg[i].cellno = i;
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
	printf("\n Parametrii %s %s %s %s %s \n",argv[1],argv[2],argv[3],argv[4],argv[5]);
	printf("\nThe program has successfully executed.\n");
	return 0;
}
