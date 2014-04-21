/*
 * p_hello.c -- a hello program (in pthread)
 */
#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX_THREAD 1000

typedef struct {
	int id;
} parameter;

void *hello(void *arg)
{
	parameter *p=(parameter *)arg;
	printf("Hello from node %d\n", p->id);
	return;
}

void main(int argc, char* argv[]) {
	int n,i;
	pthread_t *threads;
	pthread_attr_t pthread_custom_attr;
	parameter *p;

	n = 10;

	threads=(pthread_t *)malloc(n*sizeof(*threads));

	p=(parameter *)malloc(sizeof(parameter)*n);

	for (i=0; i<n; i++)
	{
		p[i].id=i;
		pthread_create(&threads[i], NULL, hello, (void *)(p+i));
	}


	for (i=0; i<n; i++)
	{
		pthread_join(threads[i],NULL);
	}
	free(p);
}
w
