#include "dolang.h"
#include "safe_alloc.h"
#include "array.h"
#include "string.h"
#include "fn.h"

do_string_construct( variable *ths, variable *str ) {
	array_set(ths, dostring("value"), str );

	array_set( ths, dostring("length"), do_string_length(ths) );

	return ths;
}


do_string_length( variable *ths ) {
	variable *str = array_get(ths, dostring("value"));
	return doint(string_len( str->val ));
}

do_string_substr( variable *ths, variable *offset, variable *len ) {

	variable *str = array_get(ths, dostring("value"));

	return dostring( string_substr( str->val, offset->val, len->val ) );
}

do_string_charCodeAt(variable *ths, variable *index) {

	variable *str = array_get(ths, dostring("value"));
	char b = *(char *)string_substr( str->val, index->val, 1 );

	return doint(b);
}

do_string_charAt(variable *ths, variable *index) {

	variable *str = array_get(ths, dostring("value"));
	char *a = str->val;
	char * b = safe_alloc_new( &alloc, 2);
	*b++ = *(char *)string_substr( str->val, index->val, 1 );
	*b++ = '\0';

	return dostring(b-2);
}

do_string_fromCharCode(variable *ths, variable *index) {
	char * b = safe_alloc_new( &alloc, 2);
	*b++ = index->val;
	*b++ = '\0';
	return dostring(b-2);
}

//this function should modify
do_string_indexOf(variable *ths, variable *s) {
	variable *str = array_get(ths, dostring("value"));

	string *nstr = newstring("");
	string_line( str->val, nstr );

	char *s1, *s2;
	s1 = s2 = string_val(s->val);

	int it = 0;
	char *ssr, *ssr1;

	for( int i = 0; i < nstr->i; i++ ) {
		char *sstr = nstr->strings[i];
		while( *sstr ) {
			if( *sstr == *s1 ) {
				char *sstr1 = sstr;
				while( *s1 ) {
					if( *s1++ != *sstr1++  ) {
						s1 = s2;
						break;
					}
				}
				if( *s1 == '\0' ) {
					return doint(it);
				}
			}

			sstr++;
			it++;
		}
	}
	return doint(0);
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
