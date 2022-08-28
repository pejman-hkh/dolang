
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

do_charCodeAt(variable *ths, variable *index) {
	char *str = ths->val;
	printf("%s\n", str );
	char a = ( (int)str + (int)index->val );
	dovar( ret, a, DOTYPE_INT );
	return ret;
}


load_string_class() {

    array_set1( &sym_stk, "String%fn%length", &do_string_length);
    array_set1( &sym_stk, "String%fn%substr", &do_string_substr);
    array_set1( &sym_stk, "String%fn%charCodeAt", &do_charCodeAt);

}