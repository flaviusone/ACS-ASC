#include <stdio.h>
#include <stdint.h>

typedef struct {
	int x, y;
} coord_t;

//TODO definiti cache-ul; care este tipul elementelor din cache?
#define CACHE_NAME MY_CACHE
#define CACHED_TYPE coord_t

#include <cache-api.h>


int main(unsigned long long speid, unsigned long long argp, unsigned long long envp)
{
	speid = speid;			// silence warning
    unsigned long long start_addr = argp;
    int x, y;
	coord_t* v = (coord_t*)(uint32_t)argp; // adresa de start a labirintului
	int M = (int)envp;		// dimensiunea unei linii din labirint

	//TODO implementati cautarea comorii folosind software cache
    coord_t elem = cache_rd(MY_CACHE, v);
    x = y = 0;
    while(1) {
        printf("primit %d %d\n",elem.x, elem.y);
        if (elem.x == 0 && elem.y == 0) {
            coord_t gasit;
            gasit.x = x;
            gasit.y = y;
            cache_wr(MY_CACHE, start_addr, gasit);
            break;
        } else {
            x = elem.x;
            y = elem.y;
            elem = cache_rd(MY_CACHE, v + M * elem.x + elem.y);
        }
    }
    cache_flush(MY_CACHE);
	return 0;
}

