#include "dolang.h"
#include "safe_alloc.h"
#include "array.h"
#include "fn.h"

do_array_construct( variable *ths, variable *arr ) {
	variable * v1 = donvar( "value", DOTYPE_STRING );
	array_set(ths, v1, arr );
	variable * lenstr = donvar( "length", DOTYPE_STRING );
	variable *len = do_array_length(ths);
	array_set( ths, lenstr, len );

}

do_array_test( variable *ths ) {
	variable * ret = donvar( "test", DOTYPE_STRING );
	return ret;
}

do_array_length( variable *ths ) {

	variable * v1 = donvar( "value", DOTYPE_STRING );
	variable *arr = array_get(ths, v1);


	array *arr1 = arr->val;
	variable * ret = donvar( arr1->length, DOTYPE_INT );
	return ret;
}

extern load() {
	array *arr = malloc( sizeof( array *) );
	array_init( arr );
	array_set1( arr, "Array%fn%construct", &do_array_construct );
	array_set1( arr, "Array%fn%test", &do_array_test );
	array_set1( arr, "Array%fn%length", &do_array_length );


	return arr;
}
