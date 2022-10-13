#include "dolang.h"
#include "safe_alloc.h"
#include "array.h"
#include "fn.h"

do_math_construct( variable *ths, variable *str ) {
	array_set(ths, dostring("value"), str );

	return ths;
}


extern load() {
	array *arr = malloc( sizeof( array *) );
	array_init( arr );
	array_set1( arr, "Math%fn%construct", &do_math_construct);

	
	return arr;
}
