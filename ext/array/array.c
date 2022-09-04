#include "dolang.h"
#include "safe_alloc.h"
#include "array.h"
#include "fn.h"

do_array_test( variable *ths ) {

	dovar(ret, "test", DOTYPE_STRING );
	return ret;
}


extern load() {
	array *arr = malloc( sizeof( array *) );
	array_init( arr );
	array_set1( arr, "Array%fn%test", &do_array_test );


	return arr;
}
