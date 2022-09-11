#include "dolang.h"
#include "safe_alloc.h"
#include "array.h"
#include "fn.h"

do_array_construct( variable *ths ) {

}

do_array_test( variable *ths ) {

	dovar(ret, "test", DOTYPE_STRING );
	return ret;
}

do_array_length( variable *ths ) {

	array *arr = ths->val;

	dovar(ret, arr->length, DOTYPE_INT);
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
