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
	struct img *image;
} thread_arg_t;

typedef struct {
	int x, y;
} test_t;

void *ppu_pthread_function(void *thread_arg) {

	spe_context_ptr_t ctx;
	thread_arg_t *arg = (thread_arg_t *) thread_arg;
	test_t test;
	test.x = 12;
	test.y = 13;

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
	if (spe_context_run(ctx, &entry, 0, (void *)arg->image->pixels, (void *) &test, NULL) < 0) {
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
	int i, j;
	pthread_t threads[MAX_SPU_THREADS];
	thread_arg_t thread_arg[MAX_SPU_THREADS];

	/*----------------------------------Vars---------------------------------*/
	int mod, num_spus;
	struct img image, image2;
	struct c_img c_image;
	// struct timeval t1, t2, t3, t4;
	// double total_time = 0, scale_time = 0;
	/*----------------------------------Vars---------------------------------*/

	/*-------------------------------Read Image------------------------------*/
	mod = atoi(argv[1]);
	num_spus = atoi(argv[2]);
	read_pgm(argv[3], &image);

	// for(i=0;i<image.width;i++){
	// 	for(j=0;j<image.height;j++){
	// 		printf("%d ",image.pixels[i*image.width+j]);
	// 	}
	// 	printf("\n");
	// }

	/*
	 * Create several SPE-threads to execute 'simple_spu'.
	 */
	for(i = 0; i < num_spus; i++) {

		/* Create thread for each SPE context */
		thread_arg[i].cellno = i;
		thread_arg[i].image = &image;
		if (pthread_create (&threads[i], NULL, &ppu_pthread_function, &thread_arg[i]))  {
			perror ("Failed creating thread");
			exit (1);
		}
	}

	/* Wait for SPU-thread to complete execution.  */
	for (i = 0; i < num_spus; i++) {
		if (pthread_join (threads[i], NULL)) {
			perror("Failed pthread_join");
			exit (1);
		}
	}

	printf("\n Parametrii %d %d %s %s %s \n",mod,num_spus,argv[3],argv[4],argv[5]);
	printf("\nThe program has successfully executed.\n");
	return 0;
}
