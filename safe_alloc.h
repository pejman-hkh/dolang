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
}


void * safe_alloc_new( safe_alloc *a, int size ) {

	void *ret = malloc( size );
	
	return ret;
}

void * safe_alloc_get( safe_alloc *a ) {

}

void safe_free( safe_alloc *a) {

}


#endif