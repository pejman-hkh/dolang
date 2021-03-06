#include <sys/time.h>

do_debug( variable * a, variable * b ) {
	*b = *a;
}


do_microtime() {

}

do_set_val( variable *a, variable *b ) {
	*b = *a;
}

do_print_array( variable *arr1 ) {
	array *arr = arr1->val;
	printf("[");
	char *pre = "";
	for( int i = 0; i < arr->length; i++ ) {
		printf("%s", pre);
		do_print( arr->key[i] );
		printf(":");
		do_print( arr->value[i] );
		pre = ",";
	}
	printf("]");
}


void do_print( variable *a ) {
	//printf("%d\n", a->type );
	if( a->type == 1 ) {
		printf("%s", a->val );
	} else if( a->type == 2 ) {
		printf("%d", a->val );
	} else if( a->type == 3 ) {
		do_print_array(a);
	} else {
		printf("%d", a );
	}
	fflush(stdout);
}

void do_typeof( variable *a ) {
	//printf("%d\n", a->type );

	variable *v = safe_alloc_new( &alloc, sizeof(variable) );
	v->val = a->type;
	v->type = 2;

	return v;
}

do_fn_plus_plus( variable *a ) {
	a->val += 1;
}

variable *do_fn_add( variable *a, variable *b ) {
	variable *var = safe_alloc_new( &alloc, sizeof(variable));

	if( a->type == 1 && b->type == 2 ) {
		int x = b->val;
		int length = snprintf( NULL, 0, "%d", x );
		char* str = safe_alloc_new( &alloc, length + 1 );
		snprintf( str, length + 1, "%d", x );

		var->val = mstrcat( str, a->val );
		var->type = 1;
	} else if( a->type == 2 && b->type == 1 ) {

		int x = a->val;
		int length = snprintf( NULL, 0, "%d", x );
		char* str = safe_alloc_new( &alloc, length + 1 );
		snprintf( str, length + 1, "%d", x );

		var->val = mstrcat( b->val, str );
		var->type = 1;

	} else if( a->type == 2 ) {
		var->val = (int)a->val+(int)b->val;
		var->type = 2;
	} else if( a->type == 1 ) {

		var->val = mstrcat( b->val, a->val );
		var->type = 1;
	}

	return var;
}

do_to_var( int a ) {
	variable *var = safe_alloc_new( &alloc, sizeof(variable));

	var->val = a;
	var->type = 2;

	return var;
}


do_fn_equal_equal( variable *a, variable *b ) {

	if( b->type == 2 ) {
		if( (int)a->val == (int)b->val) {
			return 1;
		}
	} if( b->type == 1 ) {
		int cmp = strcmp( a->val, b->val );
		if( cmp == 0 ) {
			return 1;
		}
	}

	return 0;

}
