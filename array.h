#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
	void ** key;
	void ** value;
	int length;
	//int * type;
} array;

variable * array_get( variable *arr1, variable *key) ;


array_init( array *arr ) {
	arr->length = 0;
	arr->key = malloc( sizeof(variable)  * (arr->length+2) );
	arr->value = malloc( sizeof(variable)  * (arr->length+2) );
}

array_relloc( array *arr ) {
	arr->key = realloc(arr->key, sizeof(variable) * (arr->length+2) );
	arr->value = realloc(arr->value, sizeof(variable) * (arr->length+2) );
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


	//do_print( arr1 );

	if( ! exists ) 
		array_relloc( arr );


	arr->key[ index ] = key;
	arr->value[ index ] = value;	

	if( ! exists ) {
		arr->length++;
	}
}

variable * array_get( variable *arr1, variable *key ) {
	array *arr = arr1->val;
	for( int i = 0; i < arr->length; i++ ) {
		variable *hkey = arr->key[i];

		if( do_fn_equal_equal( key, hkey ) ) {
			return arr->value[i];
		}
	}
	variable *k = safe_alloc_new( &alloc, sizeof(variable) );
	*k = *key;

	variable *v = safe_alloc_new( &alloc, sizeof(variable) );
	v->val = safe_alloc_new( &alloc, sizeof(variable) );
	v->type = 1;

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

/*variable *array_iter( array *arr ) {
	return arr->i++ < arr->length;
}*/

void * set_val( variable *a, variable *b ) {
/*	a->type = b->type;

	memcpy( a->val, b->val );
*/
	
	//do_print(a);

	//do_print(b);

	*a = *b;
	//*(a->val) = *(b->val);

	//memcpy(a,b,sizeof(a));
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