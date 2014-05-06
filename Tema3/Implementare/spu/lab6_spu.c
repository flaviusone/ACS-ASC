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
unsigned int avg[64] __attribute__ ((aligned(16)));

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
	short int cellno,width,height,no_rows,no_reps,contor;
	short int value;
	int i,j;
	unsigned long long payload = (unsigned long long)envp;

	cellno = (payload&0x000000FF);
	width = (payload&0x0000FF00) >> 8;
	height = (payload >> 16);
	no_rows = height/8;
	no_reps = width/8;
	contor=0;
	unsigned int average;

	// value = cache_rd(CACHE, start);

	// printf("Hello World! from Cell no %d, width=%d , height=%d first_val = %d\n", cellno, width, height, value);

	/* -------------------------------------Compression part -------------------------------------*/
	while(contor<no_reps){
		/* Umplu blockul cu date din cache */
		for(i = 0 ; i < 8 ; i++){
			for(j = 0 ; j < 8 ; j++){
				block[i*8+j] = cache_rd(CACHE,start + cellno * (no_rows-1) * width * 8 + i*height + j + contor*8);
			}
		}

		sleep(cellno);
		/* Cast la vector */
		vector unsigned int *vblock = (vector unsigned int *) block;

		/* Calculez avg */
		average = 0;
		for(i = 0 ; i < 64 ; i++){
			average+=block[i];
		}
		average /= 64;

		/* Umplu un vector cu avg */
		for(i = 0 ; i < 64 ; i++){
			avg[i] = average;
		}

		/* Cast la vector */
		vector unsigned int *vavg = (vector unsigned int *) avg;
		
		printf("VBlock\n\n");
		for (i=0; i<64; i++)
     	printf("%d ", vblock[i]);
		printf("Boom\n\n");
		for (i=0; i<64; i++)
     	printf("%d ", vavg[i]);

     
		/* Pt debug */
		printf("Sunt %d si am:\n", cellno);
		for(i = 0 ; i < 8 ; i++){
			for(j = 0 ; j < 8 ; j++){
				printf("%d ",block[i*8+j] );
			}
			printf("\n");
		}
		printf("Repeat\n");
	contor++;
	}

	cache_flush(CACHE);
	// printf("Done\n");
	return 0;
}




