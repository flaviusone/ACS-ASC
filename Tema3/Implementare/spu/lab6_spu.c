/*
 * Computer System Architecture
 * SPU code
 */
#include <stdio.h>
#include <spu_intrinsics.h>
#include <stdint.h>
#include <spu_mfcio.h>
#include "btc.h"

typedef struct {
	int x, y;
} test_t;
// typedef struct{
// 	//regular image
// 	int width, height;
// 	short int* pixels;
// }img;

// #define CACHE_NAME CACHE
// #define CACHED_TYPE short int
// #define CACHE_STATS              /* activare stats */

// #include <cache-api.h>


int main(unsigned long long speid,
		unsigned long long argp,
		unsigned long long envp)
{
	// struct img *image = (struct img*)argp;
	short int* start = (short int*)(uint32_t)argp;
	short int value;
	uint32_t width,height;

	printf("DO YOU EVEN LIFT BRO ?\n");
	while (spu_stat_in_mbox()<=0); // busy-waiting...

	width = spu_read_in_mbox();
	printf("YA BRAH ?\n");

	// while (spu_stat_in_mbox()<=0); // busy-waiting...
	// height = spu_read_in_mbox();

	// value = cache_rd(CACHE, start);
	// printf("Hello World! from Cell no %lld,width=%d first_val = %d\n", envp, width, value);
	// cache_flush(CACHE);
	printf("Done\n");
	return 0;
}




