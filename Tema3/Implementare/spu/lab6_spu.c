/*
 * Computer System Architecture
 * SPU code
 */
#include <stdio.h>
#include <spu_intrinsics.h>
#include <stdint.h>
#include "btc.h"

typedef struct {
	int x, y;
} test_t;
// typedef struct{
// 	//regular image
// 	int width, height;
// 	short int* pixels;
// }img;

#define CACHE_NAME CACHE
#define CACHED_TYPE short int
#define CACHE_STATS              /* activare stats */

#include <cache-api.h>


int main(unsigned long long speid,
		unsigned long long argp,
		unsigned long long envp)
{
	// struct img *image = (struct img*)argp;
	short int* start = (short int*)(uint32_t)argp;
	short int value;
	value = cache_rd(CACHE, start);
	printf("Hello World! from Cell no %lld, id (0x%llx) first_val = %d\n", envp, speid,value);
	cache_flush(CACHE);
	printf("Done\n");
	return 0;
}




