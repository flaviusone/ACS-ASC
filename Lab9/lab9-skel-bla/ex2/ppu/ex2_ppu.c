#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <libspe2.h>
#include <pthread.h>

extern spe_program_handle_t ex2_spu;

#define N 800	// numarul de linii
#define M 200	// numarul de coloane
#define NR_JUMPS 50

typedef struct {
	int x, y;
} coord_t;

coord_t v[N][M] __attribute__((aligned(16)));
coord_t destination;  // coordonatele comorii

// construieste drumul in labirint
static void init_matrix()
{
	int k;
	coord_t c, newc;
	c.x = c.y = 0;

	for(k = 0; k < NR_JUMPS; ++k) {
		newc.x = rand() % N;
		newc.y = rand() % M;
		if((v[c.x][c.y].x == 0) && (v[c.x][c.y].y == 0)) {
			v[c.x][c.y] = newc;
			printf("newc:x=%d, y=%d\n", newc.x, newc.y);
			c = newc;
		}
	}
	destination = newc;
}

int main()
{
	spe_context_ptr_t ctx;
	unsigned int entry = SPE_DEFAULT_ENTRY;
	init_matrix();

	if((ctx = spe_context_create(0, NULL)) == NULL) {
		perror ("Failed creating context");
		exit (1);
	}

	if(spe_program_load(ctx, &ex2_spu)) {
		perror ("Failed loading program");
		exit (1);
	}

	printf("SPU:\n");
	if(spe_context_run(ctx, &entry, 0, (void*)&v, (void*)M, NULL) < 0) {
		perror ("Failed running context");
		exit (1);
	}

	if(spe_context_destroy(ctx) != 0) {
		perror("Failed destroying context");
		exit (1);
	}

	printf("PPU:\n");
	printf("received: %d %d \n", v[0][0].x, v[0][0].y);
	printf("correct: %d\n", (destination.x == v[0][0].x) && (destination.y == v[0][0].y));

	return 0;
}

