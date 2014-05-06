/*
 * Computer System Architecture
 * SPU code
 */
#include <stdio.h>
#include <spu_intrinsics.h>
#include <stdint.h>
#include <spu_mfcio.h>
#include <unistd.h>
#include "btc.h"

typedef struct{
	//regular image
	int width, height;
	short int* pixels;
}img;

typedef struct{
	short int width;
	short int height;
	short int cellno;
} de_test;

unsigned int block[64] __attribute__ ((aligned(16)));

#define CACHE_NAME CACHE
#define CACHED_TYPE short int
#define CACHE_STATS              /* activare stats */

#include <cache-api.h>

void bin(unsigned   n)
{
    unsigned   i;
    for (i = 1 << 31; i > 0; i = i / 2)
        (n & i)? printf("1"): printf("0");
}

int main(unsigned long long speid,
		unsigned long long argp,
		unsigned long long envp)
{
	// struct img *image = (struct img*)argp;
	short int* start = (short int*)(uint32_t)argp;
	short int cellno,width,height,no_rows;
	short int value;
	int i,j;
	unsigned long long payload = (unsigned long long)envp;

	cellno = (payload&0x000000FF);
	width = (payload&0x0000FF00) >> 8;
	height = (payload >> 16);
	no_rows = height/8;


	// value = cache_rd(CACHE, start);

	// printf("Hello World! from Cell no %d, width=%d , height=%d first_val = %d\n", cellno, width, height, value);

	/* -------------------------------------Compression part -------------------------------------*/

	/* Umplu blockul cu date din cache */
	for(i = 0 ; i < 8 ; i++){
		for(j = 0 ; j < 8 ; j++){
			block[i*8+j] = cache_rd(CACHE,start + cellno * (no_rows-1) * width * 8 + i*height + j);
		}
	}

	sleep(cellno);

	printf("Sunt %d si am:\n", cellno);
	for(i = 0 ; i < 8 ; i++){
		for(j = 0 ; j < 8 ; j++){
			printf("%d ",block[i*8+j] );
		}
		printf("\n");
	}


	cache_flush(CACHE);
	// printf("Done\n");
	return 0;
}




