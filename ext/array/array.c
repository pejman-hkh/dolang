#include "dolang.h"
#include "safe_alloc.h"
#include "array.h"
#include "fn.h"

do_array_construct( variable *ths, variable *arr ) {
	variable * v1 = dostring("value");
	array_set(ths, v1, arr );
	variable * lenstr = dostring("length");
	variable *len = do_array_length(ths);
	array_set( ths, lenstr, len );

}

do_array_test( variable *ths ) {
	variable * ret = dostring("test");
	return ret;
}

do_array_length( variable *ths ) {

	variable * v1 = dostring("value");
	variable *arr = array_get(ths, v1);


	array *arr1 = arr->val;
	variable * ret = doint(arr1->length);
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
