
do_string_length( variable *ths ) {
	dovar(ret, strlen( ths->val ), DOTYPE_INT );
	return ret;
}

do_string_substr( variable *ths, variable *offset, variable *len ) {


	int a = ths->val;
	a = a + offset->val;
	*(char *)(a + len->val) = '\0';

	dovar( ret, a, DOTYPE_STRING);

	return ret;
}


load_string_class() {

    array_set1( &sym_stk, "String%fn%length", &do_string_length);
    array_set1( &sym_stk, "String%fn%substr", &do_string_substr);

}