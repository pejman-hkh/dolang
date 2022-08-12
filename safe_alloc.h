#ifndef do_safealloc
#define do_safealloc

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
	int ** allocate;
	int i;
} safe_alloc;

void safe_alloc_init( safe_alloc *a ) {
	a->i = 0;
	a->allocate = malloc( sizeof(int) );
}

void * safe_alloc_new( safe_alloc *a, int size ) {
	void *ret = malloc( size );
	a->allocate = realloc(a->allocate, sizeof(int) * (a->i+1) );
	a->allocate[a->i++] = ret;
	return ret;
}

void * safe_alloc_get( safe_alloc *a ) {
	return a->allocate[a->i-1];
}

void safe_free( safe_alloc *a) {
	int * is_empty = malloc( a->i );
	for( int i = 0; i < a->i; i++ ) {
		free(a->allocate[i]);
		//printf("%d\n", a->allocate[i]);
	}
	free(a->allocate);
}


#endif