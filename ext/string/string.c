#include "dolang.h"
#include "safe_alloc.h"
#include "array.h"
#include "fn.h"

do_string_construct( variable *ths, variable *str ) {
	variable * v1 = donvar( "value", DOTYPE_STRING );
	array_set(ths, v1, str );
	variable * lenstr = donvar( "length", DOTYPE_STRING );
	variable *len = do_string_length(ths);

	array_set( ths, lenstr, len );

	return ths;
}


do_string_length( variable *ths ) {
	variable * v1 = donvar( "value", DOTYPE_STRING );
	variable *str = array_get(ths, v1);

	variable * ret = donvar( strlen( str->val ), DOTYPE_INT );
	return ret;
}

do_string_substr( variable *ths, variable *offset, variable *len ) {

	variable * v1 = donvar( "value", DOTYPE_STRING );
	variable *str = array_get(ths, v1);

	char *a = str->val;

	char *r = safe_alloc_new( &alloc, strlen( a ) + 1 );
	memcpy( r, a, strlen( a ) + 1 );

	r = (int)r + offset->val;
	*(char *)((int)r + len->val) = '\0';

	variable * ret = donvar( r, DOTYPE_STRING );

	return ret;
}

do_string_charCodeAt(variable *ths, variable *index) {
	variable * v1 = donvar( "value", DOTYPE_STRING );
	variable *str = array_get(ths, v1);
	char *a = str->val;
	char b = *(char *)( (int)a + (int)index->val );
	variable * ret = donvar( b, DOTYPE_INT );
	return ret;
}

do_string_charAt(variable *ths, variable *index) {
	variable * v1 = donvar( "value", DOTYPE_STRING );
	variable *str = array_get(ths, v1);
	char *a = str->val;
	char * b = safe_alloc_new( &alloc, 2);
	*b++ = *(char *)( (int)a + (int)index->val );
	*b++ = '\0';

	variable * ret = donvar( b-2, DOTYPE_STRING );
	return ret;
}

do_string_fromCharCode(variable *ths, variable *index) {
	char * b = safe_alloc_new( &alloc, 2);
	*b++ = index->val;
	*b++ = '\0';

	variable * ret = donvar( b-2, DOTYPE_STRING );
	return ret;
}

do_string_indexOf(variable *ths, variable *s) {

	variable * v1 = donvar( "value", DOTYPE_STRING );
	variable *str = array_get(ths, v1);
	char *a = str->val;

	char * res = strstr( a, s->val );
	int index = res - a;

	variable * ret = donvar( index, DOTYPE_INT );
	return ret;
}


extern load() {
	array *arr = malloc( sizeof( array *) );
	array_init( arr );
	array_set1( arr, "String%fn%construct", &do_string_construct);
	array_set1( arr, "String%fn%length", &do_string_length);
	array_set1( arr, "String%fn%substr", &do_string_substr);
	array_set1( arr, "String%fn%charCodeAt", &do_string_charCodeAt);
	array_set1( arr, "String%fn%charAt", &do_string_charAt);
	array_set1( arr, "String%fn%indexOf", &do_string_indexOf);
	array_set1( arr, "String%fn%fromCharCode", &do_string_fromCharCode);
	
	return arr;
}
