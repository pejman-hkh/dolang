#ifndef doarrayh
#define doarrayh 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string.h"


typedef struct {
	void ** key;
	void ** value;
	int length;
	//int * type;
} array;

variable * array_get( variable *arr1, variable *key) ;


array_init( array *arr ) {
	arr->length = 0;
	arr->key = safe_alloc_new( &alloc, sizeof(variable )  * (arr->length+2) );
	arr->value = safe_alloc_new( &alloc, sizeof(variable )  * (arr->length+2) );
}

array_relloc( array *arr ) {

	if( arr->length == 0 ) {
		array_init( arr );
	}

	arr->key = realloc(arr->key, sizeof(variable ) * (arr->length+2) );
	arr->value = realloc(arr->value, sizeof(variable ) * (arr->length+2) );
}


void *array_set( variable *arr1, variable *key, variable *value ) {
	array *arr = arr1->val;
	int index = arr->length;

	int exists = 0;
	for( int i = 0; i < arr->length; i++ ) {
		variable *hkey = arr->key[i];
		if( do_fn_equal_equal( key, hkey ) ) {
			index = i;
			exists = 1;
			break;
		}
	}

	if( ! exists ) 
		array_relloc( arr );


	arr->key[ index ] = key;
	arr->value[ index ] = value;	


	if( ! exists ) {
		arr->length++;
	}
	
}


void *array_set2( variable *arr1, variable *key, variable *value ) {
	array *arr = arr1->val;
	variable * index = doint(arr->length);
	
	array_set( arr1, index, key);
}

variable * array_get( variable *arr1, variable *key ) {
	//printf("%s\n", key->val );
	if( ! arr1->type ) {
		return donvar( "", DOTYPE_UNDEF );
	}

	if( arr1->type == DOTYPE_STRING ) {

		if( key->type == DOTYPE_INT ) {
			int index = key->val;
			if( index > string_len(arr1->val) ) {
				return doint( 0 );
			} else {

				char *d = string_index( (string *)arr1->val, (int)index );
		
				return donvar( d, DOTYPE_CHAR );
			}
		}
	}

	array *arr = arr1->val;

	for( int i = 0; i < arr->length; i++ ) {
		variable *hkey = arr->key[i];
/*		do_print(key, key);
		printf(":");
		do_print(hkey, hkey);
		printf("\n");
		printf("\n");*/
		if( do_fn_equal_equal( key, hkey ) ) {
			return arr->value[i];
		}
	}

	if( key->type == DOTYPE_STRING && string_cmp1(key->val,"prototype") == 0 ) {
		return arr1;
	}

	variable *k = safe_alloc_new( &alloc, sizeof(variable) );
	*k = *key;

	variable *val = safe_alloc_new( &alloc, sizeof(variable) );

	variable * v = donvar( val, DOTYPE_UNDEF );
	array_set( arr1, k, v );
	return v;
}

void *array_set1( array *arr, void *key, void *value ) {

	int index = arr->length;
	for( int i = 0; i < arr->length; i++ ) {

		if( strcmp( arr->key[i], key ) == 0 ) {

			arr->value[ i ] = value;

			return;
		}
	}

	array_relloc( arr );

	arr->key[ index ] = key;
	arr->value[ index ] = value;
	arr->length++;
}

void *array_set_int( array *arr, void *key, void *value ) {

	int index = arr->length;
	array_relloc( arr );

	arr->key[ index ] = key;
	arr->value[ index ] = value;
	arr->length++;
}

void * array_get1( array *arr, void *key ) {
	for( int i = 0; i < arr->length; i++ ) {
		if( strcmp( arr->key[i], key ) == 0 ) {
			return arr->value[i];
		}
	}

	return 0;
}

variable * array_key( variable *arr1, variable * index ) {

	array *arr = arr1->val;

	return arr->key[(int)index->val];
}

variable * array_value( variable *arr1, variable * index ) {
	array *arr = arr1->val;
	return arr->value[(int)index->val];
}

variable * array_len( variable *arr1 ) {
	array *arr = arr1->val;
	variable *v = safe_alloc_new( &alloc, sizeof(variable) );
	v->val = arr->length;
	v->type = 2;

	return v;
}


int array_len1( array *arr1 ) {
	return arr1->length;
}

void * array_set_val( variable *a, variable *b ) {

	if( a->type == 5 ) {
		memcpy( a->val, string_val(b->val), 1 );
	} else {
		*a = *b;
	}

}

void *array_reset(array *arr) {
	for( int i = 0; i < arr->length; i++ ) {
		arr->key[i] = 0;
		arr->value[i] = 0;
	}
	arr->length = 0;
}


array_print( array *arr ) {
	for( int i = 0; i < arr->length; i++ ) {
		printf("%s : %d\n", arr->key[i], arr->value[i] );
	}
}

void * array_free( array *arr ) {
	arr->length = 0;
	free(arr->key);
	free(arr->value);
}

#endif