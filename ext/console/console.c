#include "dolang.h"
#include "safe_alloc.h"
#include "array.h"
#include "fn.h"

do_console_construct( variable *ths ) {
	//array_set(ths, dostring("value"), num );

	return ths;
}

do_console_log( variable *ths, variable *in ) {
	do_print( ths, in );

	return ths;
}

do_console_error( variable *ths, variable *in ) {
	do_print( ths, in );

	return ths;
}

extern load() {
	array *arr = malloc( sizeof( array *) );
	array_init( arr );
	array_set1( arr, "console%fn%construct", &do_console_construct);
	array_set1( arr, "console%fn%log", &do_console_log);
	array_set1( arr, "console%fn%error", &do_console_error);

	
	return arr;
}
