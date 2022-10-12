#include "dolang.h"
#include "safe_alloc.h"
#include "array.h"
#include "fn.h"

do_array_construct( variable *ths, variable *arr ) {
	array_set(ths, dostring("value"), arr );
	variable *len = do_array_length(ths);
	array_set( ths, dostring("length"), len );

}

do_array_test( variable *ths ) {

	return dostring("test");
}

do_array_length( variable *ths ) {
	variable *arr = array_get(ths, dostring("value"));
	array *arr1 = arr->val;
	return doint(arr1->length);
}

extern load() {
	array *arr = malloc( sizeof( array *) );
	array_init( arr );
	array_set1( arr, "Array%fn%construct", &do_array_construct );
	array_set1( arr, "Array%fn%test", &do_array_test );
	array_set1( arr, "Array%fn%length", &do_array_length );


	return arr;
}
