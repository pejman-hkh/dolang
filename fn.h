#ifndef do_fn
#define do_fn
#include <sys/time.h>
#include <unistd.h>

do_fn_new_regex( variable *a, variable *b ) {

	int l = RegExpClass;

	variable *cls = do_fn_new_class( l );
	dovar(pk, "prototype", DOTYPE_STRING);
	variable *proto = array_get( cls, pk );
	dovar(fnc, "construct", DOTYPE_STRING);
	variable *ret = array_get( proto, fnc );

	typedef variable *( *fn )( variable *ths, variable *a, variable *b );

	fn construct = ret->val;

	return construct( cls, a, b );

}

do_fn_new_class( variable *cls ) {

	array *arr = safe_alloc_new(&alloc, sizeof(array));
	array_init( arr );

	dovar( ret, arr, DOTYPE_OBJECT );


	array *arr1 = cls->val;
	for( int i = 0; i < arr1->length; i++ ) {
		variable *key = arr1->key[i];
		variable *value = arr1->value[i];
		dovar(k,key->val, key->type);
		dovar(v,value->val, value->type);
		array_set( ret, k, v );
	}

	return ret;
}

do_sleep( variable *ths, variable *t ) {
	fflush(stdout);

	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	long r = currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;	

	while( 1 ) {

		struct timeval c1;
		gettimeofday(&c1, NULL);
		long r1 = c1.tv_sec * (int)1e6 + currentTime.tv_usec;

		if( r1 - r >= (int)t->val * 1000 ) {
			break;
		}

	}
}

do_debug( variable *a) {
	printf("%s\n", a->val );
	exit(0);
}


do_microtime() {
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	long r = currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
	dovar(ret, r, DOTYPE_LONG);
	return ret;
}

do_set_val( variable *a, variable *b ) {
	*b = *a;
	return a;
}

do_print_array( variable *ths, variable *arr1 ) {
	array *arr = arr1->val;
	printf("[");
	char *pre = "";
	for( int i = 0; i < arr->length; i++ ) {
		printf("%s", pre);
		do_print( ths, arr->key[i] );
		variable *k = arr->key[i];

		printf(":");
		do_print( ths, arr->value[i] );
		
		pre = ",";
	}
	printf("]\n");
}


void do_print( variable *ths, variable *a ) {

	if( a->type == 0 ) {
	} else if( a->type == 1 ) {
		printf("%s", a->val );
	} else if( a->type == 2 ) {
		printf("%d", a->val );
	} else if( a->type == 3 ) {
		do_print_array(ths, a);	
	} else if( a->type == 11 ) {
		do_print_array(ths, a);	
	} else if( a->type == 4 ) {
		printf("func()");
	} else if( a->type == 5 ) {
		char * r = safe_alloc_new( &alloc, sizeof( char *) );
		memcpy( r, a->val, 2);
		*(r+1) = '\0';		
		printf("%s", r );
	} else if( a->type == 6 ) {
		printf("%ld", a->val );
	} else if( a->type == 7 ) {
		printf("#res");
	} else if( a->type == 9 ) {
		printf("undefined");
	} else {
		printf("%d", a );
	}
	fflush(stdout);
}

void do_typeof( variable *ths, variable *a ) {
	//printf("%d\n", a->type );

	dovar(v,a->type, 2);

/*	variable *v = safe_alloc_new( &alloc, sizeof(variable) );
	v->val = a->type;
	v->type = 2;
*/
	return v;
}

do_fn_plus_plus( variable *a ) {
	a->val += 1;
	dovar(ret, a->val - 1, DOTYPE_INT);
	return ret;
}

do_fn_minus_minus( variable *a ) {
	a->val -= 1;
}

int do_fn_shift_left( int a, int b ) {
	int r = (int)b << (int)a;
	return r;
}

int do_fn_shift_right( int a, int b ) {
	int r = (int)b >> (int)a;
	return r;
}

variable *do_fn_add( variable *a, variable *b ) {
	variable *var = safe_alloc_new( &alloc, sizeof(variable));

	if( a->type == 1 && b->type == 5 ) {
		char * r = safe_alloc_new( &alloc, sizeof( char *) );
		memcpy( r, b->val, 2);
		*(r+1) = '\0';

		var->val = mstrcat( r, a->val );
		var->type = 1;

	} else if( a->type == 5 && b->type == 1 ) {

		char * r = safe_alloc_new( &alloc, sizeof( char *) );
		memcpy( r, a->val, 2);
		*(r+1) = '\0';

		var->val = mstrcat( b->val, r );
		var->type = 1;


	} else if( a->type == 1 && b->type == 2 ) {
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

	if( b->type == 5 ) {
		char *g = safe_alloc_new( &alloc, 2 );
		memcpy( g, b->val, 1 );
		g[1] = '\0';

		if( strcmp( a->val, g ) == 0 ) {
			return 1;
		}
		return 0;
	}

	if( a->type == 5 ) {
		char *g = safe_alloc_new( &alloc, 2 );
		memcpy( g, a->val, 1 );
		g[1] = '\0';

		if( strcmp( g, b->val ) == 0 ) {
			return 1;
		}

		return 0;
	}

	if( b->type == 2 ) {
		if( (int)a->val == (int)b->val) {
			return 1;
		}
	} 

	if( b->type == 1 ) {
		int cmp = strcmp( a->val, b->val );
		if( cmp == 0 ) {
			return 1;
		}
	}

	return 0;

}


do_fn_equal_equal1( variable *a, variable *b ) {
	int a1 = do_fn_equal_equal( a, b);
	dovar(ret, a1, DOTYPE_INT);
	return ret;

}

do_fn_not_equal( variable *a, variable *b ) {

	int a1 = do_fn_equal_equal( a, b);
/*	printf("%d\n", a->val);
	printf("%d\n", b->val);
	printf("%d\n", !a1);
	printf("tttttttttttttt\n");*/
	dovar(ret, !a1, DOTYPE_INT);
	return ret;

}

#endif