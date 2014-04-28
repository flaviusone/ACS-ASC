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
	int M = (int)envp;		// dimensiunea unei linii din labirint

	// start block
	coord_t* start = (coord_t*)(uint32_t)argp;
	coord_t block = cache_rd(MY_CACHE, start);
	coord_t old;
	//TODO implementati cautarea comorii folosind software cache
	while(1) {
		// daca nu am ajuns la start citesc alt block
        if (block.x != 0 && block.y != 0) {
			old.x = block.x;
            old.y = block.y;
            block = cache_rd(MY_CACHE, start + M * block.x + block.y);
        } else {
            coord_t end;
            end.x = old.x;
            end.y = old.y;
            cache_wr(MY_CACHE, start_addr, end);
            break;
        }

	}


	cache_flush(MY_CACHE);
	return 0;
}

