#include "dolang.h"
#include "safe_alloc.h"
#include "array.h"
#include "fn.h"

do_string_length( variable *ths ) {
	dovar(ret, strlen( ths->val ), DOTYPE_INT );
	return ret;
}

do_string_substr( variable *ths, variable *offset, variable *len ) {
	char *a = ths->val;
	char *r = safe_alloc_new( &alloc, strlen( ths->val ) + 1 );
	memcpy( r, a, strlen( ths->val ) + 1 );

	r = (int)r + offset->val;
	*(char *)((int)r + len->val) = '\0';

	dovar( ret, r, DOTYPE_STRING);

	return ret;
}

do_string_charCodeAt(variable *ths, variable *index) {
	char *str = ths->val;
	char a = ( (int)str + (int)index->val );
	dovar( ret, a, DOTYPE_INT );
	return ret;
}


do_string_indexOf(variable *ths, variable *s) {
	char *str = ths->val;
	char * res = strstr( str, s->val );
	int index = res - str;

	dovar( ret, index, DOTYPE_INT );
	return ret;
}


extern load() {
	array *arr = malloc( sizeof( array *) );
	array_init( arr );
	array_set1( arr, "String%fn%length", &do_string_length);

	array_set1( arr, "String%fn%substr", &do_string_substr);
	array_set1( arr, "String%fn%charCodeAt", &do_string_charCodeAt);
	array_set1( arr, "String%fn%indexOf", &do_string_indexOf);
	
	return arr;
}
