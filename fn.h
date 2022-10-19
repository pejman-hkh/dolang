#ifndef do_fn
#define do_fn
#include <sys/time.h>
#include <unistd.h>

do_dot_init( variable *in ) {
	//printf("%d\n", in->type );
	if( in->type == DOTYPE_ARRAY ) {
		variable *cls = do_fn_new_class( ArrayClass );

		variable *pt = array_get( cls, dostring("prototype") );

		typedef variable *( *fn )( variable *ths, variable *arr);
		variable *r = array_get(pt, dostring("construct") );
		fn construct = r->val;
		construct(cls, in);		

		return cls;
	} else if( in->type == DOTYPE_STRING ) {
		variable *cls = do_fn_new_class( StringClass );

		variable *pt = array_get( cls, dostring("prototype") );

		typedef variable *( *fn )( variable *ths, variable *str);

		variable *r = array_get(pt, dostring("construct") );
		fn construct = r->val;
		construct(cls, in);
		return cls;
	} else {
		return in;
	}
}

do_fn_new_regex( variable *a, variable *b ) {

	variable *cls = do_fn_new_class( RegExpClass );
	variable *proto = array_get( cls, dostring("prototype") );
	variable *ret = array_get( proto, dostring("construct") );

	typedef variable *( *fn )( variable *ths, variable *a, variable *b );

	fn construct = ret->val;

	return construct( cls, a, b );

}

do_fn_new_class( variable *cls ) {
	variable * ret = doobject();

	array *arr1 = cls->val;
	for( int i = 0; i < arr1->length; i++ ) {
		variable *key = arr1->key[i];
		variable *value = arr1->value[i];

		array_set( ret, donvar( key->val, key->type ), donvar( value->val, value->type ) );
	}

	//do_print( ret, ret );

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

do_debug1( variable *a) {
	printf("in debug1 : \n");
	do_print(a,a);
	//exit(0);
}

do_debug( variable *a) {
	printf("in debug : \n");
	do_print(a,a);
	//exit(0);
	//return a;
}

do_exit() {
	exit(0);
}

do_microtime() {
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	long r = currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
	variable * ret = donvar( r, DOTYPE_LONG );
	return ret;
}

do_set_val( variable *a, variable *b ) {

	*b = *a;
	return a;
}

print_array( variable *ths, variable *arr1, char *s, char *e ) {
	array *arr = arr1->val;
	printf(s);
	char *pre = "";
	for( int i = 0; i < arr->length; i++ ) {
		printf("%s", pre);
		do_print( ths, arr->key[i] );
		variable *k = arr->key[i];

		printf(":");
		do_print( ths, arr->value[i] );
		
		pre = ",";
	}
	printf(e);	
	printf("\n");	
}

do_print_array( variable *ths, variable *arr1 ) {
	print_array( ths, arr1, "[", "]");
}

do_print_object( variable *ths, variable *arr1 ) {
	print_array( ths, arr1, "{", "}");
}

void do_print( variable *ths, variable *a ) {

	if( a->type == 0 ) {
	} else if( a->type == DOTYPE_STRING ) {
		string_print( a->val );
	} else if( a->type == DOTYPE_INT ) {
		printf("%d", a->val );
	} else if( a->type == DOTYPE_FLOAT ) {
		printf("%f", a->floatVal );
	} else if( a->type == DOTYPE_ARRAY ) {
		do_print_array(ths, a);	
	} else if( a->type == DOTYPE_OBJECT ) {
		do_print_object(ths, a);	
	} else if( a->type == DOTYPE_FUNC ) {
		printf("func()");
	} else if( a->type == DOTYPE_CHAR ) {
		char * r = safe_alloc_new( &alloc, sizeof( char *) );
		memcpy( r, a->val, 2);
		*(r+1) = '\0';		
		printf("%s", r );
	} else if( a->type == DOTYPE_LONG ) {
		printf("%ld", a->val );
	} else if( a->type == DOTYPE_RES ) {
		printf("#res");
	} else if( a->type == DOTYPE_UNDEF ) {
		printf("undefined");
	} else {
		printf("%d", a );
	}
	fflush(stdout);
}

void do_typeof( variable *ths, variable *a ) {
	return doint( a->type );
}

do_fn_plus_plus( variable *a ) {
	a->val += 1;
	return doint(a->val - 1);
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

	if( a->type == DOTYPE_FLOAT && b->type == DOTYPE_FLOAT ) {
		float c = (float)(a->floatVal) + (float)( b->floatVal );
		return dofloat( c );
	} else if( a->type == 1 && b->type == DOTYPE_CHAR ) {
		char * r = safe_alloc_new( &alloc, sizeof( char *) );
		memcpy( r, b->val, 2);
		*(r+1) = '\0';

		var->val = string_cat2( r, a->val );
		var->type = 1;

	} else if( a->type == DOTYPE_CHAR && b->type == DOTYPE_STRING ) {

		char * r = safe_alloc_new( &alloc, sizeof( char *) );
		memcpy( r, a->val, 2);
		*(r+1) = '\0';

		var->val = string_cat1( b->val, r );
		var->type = 1;

	} else if( a->type == DOTYPE_STRING && b->type == DOTYPE_INT ) {
		int x = b->val;
		int length = snprintf( NULL, 0, "%d", x );
		char* str = safe_alloc_new( &alloc, length + 1 );
		snprintf( str, length + 1, "%d", x );

		var->val = string_cat2( str, a->val );
		var->type = 1;
	} else if( a->type == DOTYPE_INT && b->type == DOTYPE_STRING ) {

		int x = a->val;
		int length = snprintf( NULL, 0, "%d", x );
		char* str = safe_alloc_new( &alloc, length + 1 );
		snprintf( str, length + 1, "%d", x );

		var->val = string_cat1( b->val, str );
		var->type = 1;

	} else if( a->type == DOTYPE_INT ) {
		var->val = (int)a->val+(int)b->val;
		var->type = 2;
	} else if( a->type == DOTYPE_STRING ) {

		var->val = string_cat( b->val, a->val );
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

	if( b->type == DOTYPE_CHAR ) {
		char *g = safe_alloc_new( &alloc, 2 * sizeof( char * ) );
		memcpy( g, b->val, 1 );
		g[1] = '\0';

		if( string_cmp1( a->val, g ) == 0 ) {
			return 1;
		}
		return 0;
	}

	if( a->type == DOTYPE_CHAR ) {
		char *g = safe_alloc_new( &alloc, 2 * sizeof( char *) );
		memcpy( g, a->val, 1 );
		g[1] = '\0';

		if( string_cmp2( g, b->val ) == 0 ) {
			return 1;
		}

		return 0;
	}

	if( b->type == DOTYPE_INT ) {
		if( (int)a->val == (int)b->val) {
			return 1;
		}
	} 

	if( b->type == DOTYPE_STRING ) {

		int cmp = string_cmp( a->val, b->val );

		if( cmp == 0 ) {
			return 1;
		}
	}

	return 0;

}


do_fn_equal_equal1( variable *a, variable *b ) {
	return doint( do_fn_equal_equal( a, b) );

}

do_fn_not_equal( variable *a, variable *b ) {
	return doint( ! do_fn_equal_equal( a, b) );
}

#endif