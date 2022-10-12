skip(a);
next();
block();
skipLine();
o();
print_tok();
char * mstrcat();
void mprint( variable *a );

#include "fn.h"
#include "conf.h"

do_new_let() {
	vars_init();
	ivar = ivar - 4;
	*(int *)indvar = -ivar;
	return ivar;
}


vars_init() {
	if( ivar == 0 ) {
		#if Assembly
		printf("sub $%c,%%esp\n", indvar);
		#endif
		*ind++ = 0x81;
		*ind++ = 0xec;
		indvar = ind;
		ind += 4;
	}	
}

do_get_val() {
	#if Assembly 
	printf("mov  0x4(%%eax),%%eax\n");
	#endif
	*ind++ = 0x8b;
	*ind++ = 0x40;
	*ind++ = 0x04;	
}

do_convert_to_var( type ) {
	function_init(1);
	function_set_arg(0);
	function_call( &do_to_var, "do_to_var" );
	function_end(1);

}


do_else_ternary(a) {

	#if Assembly 
	printf("jmpq $\n");
	#endif
	*ind++ = 0xe9; 
	*(int *)ind = 0;
	int b = ind;
	ind += 4;


    int n;
    while (a) {
        n = *(int *)a;
        *(int *)a = ind - a - 4;
        a = n;
    }


	return b;
}

do_ternary(a) {

	#if Assembly 
	printf("mov  0x4(%%eax),%%eax\n");
	#endif
	*ind++ = 0x8b;
	*ind++ = 0x40;
	*ind++ = 0x04;

	#if Assembly 
	printf("test %%eax,%%eax\n");
	#endif
	*ind++ = 0x85;
	*ind++ = 0xc0;

	#if Assembly 
	printf("je  0x$\n");
	#endif
	*ind++ = 0x0f;
	*ind++ =  0x84;
	*(int *)ind = 0;
	a = ind;
	ind += 4;

	return a;


}

do_call_regex() {
	function_init(2);

	do_call_string( dostring(toks.id) );
	function_set_arg(0);

	next();
	if( toks.t == TOK_IDENT ) {
		do_call_string( dostring(toks.id) );
		function_set_arg(1);

		next();
	}

	function_call( &do_fn_new_regex, "do_fn_new_regex" );
	function_end(2);
}

do_push() {
	#if Assembly 
	printf("push %%eax\n");
	#endif
	o(0x50);	
}

do_pop() {
	#if Assembly 
	printf("pop %%ecx\n");
	#endif
	o(0x59);	
}

do_add() {

	//we can use function call or write assembly for run this
	function_init(2);
	function_set_arg(0);

	#if Assembly 
	printf("mov %%ecx, %%eax\n");
	#endif
	*ind++ = 0x89;
	*ind++ = 0xc8;

	function_set_arg(1);
	function_call( &do_fn_add, "do_fn_add" );
	function_end(2);
}

do_get_var_value() {
	#if Assembly 
	printf("mov  0x4(%%eax),%%eax\n");
	#endif
	*ind++ = 0x8b;
	*ind++ = 0x40;
	*ind++ = 0x04;

	#if Assembly 
	printf("mov  0x4(%%ecx),%%ecx\n");
	#endif
	*ind++ = 0x8b;
	*ind++ = 0x49;
	*ind++ = 0x04;	
}

do_mul() {

	#if Assembly 
	printf("imul %%ecx,%%eax\n");
	#endif
	*ind++ = 0x0f;
	*ind++ = 0xaf;
	*ind++ = 0xc1;
}

do_remain() {
	#if Assembly 
	printf("xchg %%eax,%%ecx\n");
	#endif
	*ind++ = 0x91;

	#if Assembly 
	printf("cltd\n");
	#endif
	*ind++ = 0x99;

	#if Assembly 
	printf("idiv %%ecx\n");
	#endif
	*ind++ = 0xf7;
	*ind++ = 0xf9;

	#if Assembly 
	printf("xchg %%eax,%%edx\n");
	#endif
	*ind++ = 0x92;
}

do_div() {

}

do_sub() {

	#if Assembly
	printf("sub %%ecx,%%eax\n");
	#endif
	*ind++ = 0x29; 
	*ind++ = 0xc8;

	#if Assembly
	printf("neg %%eax\n");
	#endif

	*ind++ = 0xf7;
	*ind++ = 0xd8;
}

do_return() {

	#if Assembly 
	printf("jmp $\n");
	#endif
	*ind++ = 0xe9;
	ind += 4;
}


do_call_string( variable *var ) {
	#if Assembly 
	printf("------------------ string : %s \n", var->val );
	printf("mov 0x%x,%%eax\n", var->val );
	#endif
	*ind++ = 0xb8;
	*(int *)ind = var;
	ind += 4;
}

do_call_num( int id ) {
	#if Assembly 
	printf("mov 0x%x,%%eax\n", id );
	#endif
	*ind++ = 0xb8;
	*(int *)ind = id;
	ind += 4;
}

do_call_var(l) {

	if( l > 0 & l > 512 ) {
		do_call_num(l);
	} else {	
		#if Assembly 
		printf("mov %d(%%ebp),%%eax\n", l);
		#endif
		*ind++ = 0x8b;
		*ind++ = 0x85;
		*(int *)ind = l;
		ind += 4;
	}

}

do_call_function_callback( l ) {

	int i = do_before_call_function();

	if( l )
		do_call_var( l );

	do_get_val();

	#if Assembly
	printf("mov %d, %%ecx\n", ind);
	#endif
	*ind++ = 0xb9;
	*(int *)ind = ind + 8;
	ind += 4;

	#if Assembly
	printf("sub %%eax,%%ecx\n", ind);
	#endif
	*ind++ = 0x29;
	*ind++ = 0xc1;


	#if Assembly
	printf("call %%eax\n");
	#endif
	*ind++ = 0xff;
	*ind++ = 0xd0;

	do_after_call_function( i );
}

do_before_call_function() {
	#if Assembly 
	printf("sub $,%%esp\n");
	#endif
	*ind++ = 0x81; 
	*ind++ = 0xec;
	int st = ind;
	*(int *)ind =  0; 
	ind += 4;

	skip('(');
	int i = 0;

	#if Assembly 
	printf("mov %%eax,%x(%%esp)\n", i);
	#endif
	*ind++ = 0x89;
	*ind++ = 0x84;
	*ind++ = 0x24;
	*(int *)ind = i;
	ind += 4;

	i += 4;


	while( toks.c != ')' ) {
		expr();
		#if Assembly 
		printf("mov %%eax,%x(%%esp)\n", i);
		#endif
		*ind++ = 0x89;
		*ind++ = 0x84; 
		*ind++ = 0x24;
		*(int *)ind = i;
		ind += 4;

		if( toks.c == ',' )
			next();

		i += 4;
	}

	*(int *)st = i;

	skip(')');

	return i;
	
}

do_after_call_function( i ) {
	if( i ) {
		#if Assembly 
		printf("add $,%%esp\n");
		#endif
		*ind++ = 0x81;
		*ind++ =  0xc4;
		*(int *)ind = i;
		ind += 4;
	}
}

int do_main_call_function( l, bid ) {
	int ind_ret = 0;

	int n = 0;
	for( int i = 0; i <  ext.length; i++ ) {
		if( strcmp( bid, ext.key[i] ) == 0 ) {
			n = ext.value[i];
			break;
		}
	}

	if( ! n ) {
		n = dlsym(0, bid);
	}

	if( n ) {
		n = n - (int)ind - 5;
	} else {
		if( l ) {
			n = l - (int)ind - 5;
		}
	}

	#if Assembly 
	printf("call %s 0x%x\n", bid, n);
	#endif
	*ind++ = 0xe8;
	*(int *)ind = n;
	if( ! n )
		ind_ret = ind;
	ind += 4;

	return ind_ret;
}

int do_call_function( l, bid ) {

	int i = do_before_call_function();

	int ind_ret = do_main_call_function( l, bid );

	do_after_call_function( i );
	
	return ind_ret;

}

do_minus_minus() {
	//do_call_var(l+4);

	//do_call_var(l);

	//should convert to assembly ...
	function_init(1);
	function_set_arg(0);
	function_call( &do_fn_minus_minus, "do_fn_minus_minus" );
	function_end(1);


/*	function_init(1);
	function_set_arg(0);
	function_call( &do_debug, "do_debug" );
	function_end(1);
*/

/*	#if Assembly 
	printf("addl $0x%x,%d(%%ebp)\n", -1, l+4);
	#endif
	*ind++ = 0x83;
	*ind++ = 0x85;
	*(int *)ind = l+4;
	ind += 4;	
	*ind++ = -1;*/
}

do_equal_equal() {

	function_init(2);
	function_set_arg(0);

	#if Assembly 
	printf("mov %%ecx, %%eax\n");
	#endif
	*ind++ = 0x89;
	*ind++ = 0xc8;
	function_set_arg(1);

	function_call( &do_fn_equal_equal1, "do_fn_equal_equal1" );
	function_end(2);

}

do_not_equal() {
	function_init(2);
	function_set_arg(0);

	#if Assembly 
	printf("mov %%ecx, %%eax\n");
	#endif
	*ind++ = 0x89;
	*ind++ = 0xc8;
	function_set_arg(1);

	function_call( &do_fn_not_equal, "do_fn_not_equal" );
	function_end(2);


/*	#if Assembly 
	printf("cmp  %%eax,%%ecx\n");
	#endif
	*ind++ = 0x39;
	*ind++ = 0xc1;

	#if Assembly 
	printf("mov $0x0,%%eax\n");
	#endif
	*ind++ = 0xb8;
	ind += 4;

	#if Assembly 
	printf("setne %%al\n");
	#endif
	*ind++ = 0x0f;
	*ind++ = 0x95;
	*ind++ = 0xc0;*/

}

do_if_cond() {

	#if Assembly 
	printf("mov  0x4(%%eax),%%eax\n");
	#endif
	*ind++ = 0x8b;
	*ind++ = 0x40;
	*ind++ = 0x04;

	#if Assembly 
	printf("test %%eax,%%eax\n");
	#endif
	*ind++ = 0x85;
	*ind++ = 0xc0;

	#if Assembly 
	printf("je  0x$\n");
	#endif
	*ind++ = 0x0f;
	*ind++ =  0x84;
	*(int *)ind = 0;
	ind += 4;

}

do_else_cond() {
	next();
	#if Assembly 
	printf("jmpq $\n");
	#endif
	*ind++ = 0xe9; 
	*(int *)ind = 0;
	ind += 4;
	
}

do_less_than() {
	#if Assembly 
	printf("cmp %%eax,%%ecx\n");
	#endif	
	*ind++ = 0x39;
	*ind++ = 0xc1;
	#if Assembly 
	printf("mov $0x0,%%eax\n");
	#endif
	*ind++ = 0xb8;
	ind += 4;

	#if Assembly 
	printf("setl %%al\n");
	#endif
	*ind++ = 0x0f;
	*ind++ = 0x9c;
	*ind++ = 0xc0;			 	
}

do_do_while_loop(n) {

	#if Assembly 
	printf("mov  0x4(%%eax),%%eax\n");
	#endif
	*ind++ = 0x8b;
	*ind++ = 0x40;
	*ind++ = 0x04;

	#if Assembly 
	printf("test %%eax,%%eax\n");
	#endif
	*ind++ = 0x85;
	*ind++ = 0xc0;

	#if Assembly 
	printf("je $\n");
	#endif
	*ind++ = 0x0f; 
	*ind++ = 0x84; 
	*(int *)ind = 5;
	ind += 4;

	n = n - (int)ind - 5;

	#if Assembly 
	printf("jmp  $\n");
	#endif
	*ind++ = 0xe9;
	*(int *)ind = n;
	ind += 4;	
}

do_while_loop(n) {

	#if Assembly 
	printf("mov  0x4(%%eax),%%eax\n");
	#endif
	*ind++ = 0x8b;
	*ind++ = 0x40;
	*ind++ = 0x04;

	#if Assembly 
	printf("test %%eax,%%eax\n");
	#endif
	*ind++ = 0x85;
	*ind++ = 0xc0;

	#if Assembly 
	printf("je $\n");
	#endif
	*ind++ = 0x0f; 
	*ind++ = 0x84; 
	*(int *)ind = 0;
	ind += 4;


	int b = ind - 4;

	block();

	*(int *)b = (int)ind - b + 1;

	n = n - (int)ind - 5;
	#if Assembly 
	printf("jmp  $\n");
	#endif
	*ind++ = 0xe9;
	*(int *)ind = n;
	ind += 4;	
}

do_equal(l) {

	if( l > 0 & l > 512 ) {

	/*	#if Assembly 
		printf("mov %%eax,0x%x\n", l);
		#endif
		*ind++ = 0x89;
		*ind++ =  0x05;
		*(int *)ind = l;
		ind += 4;
*/
		//89 05 90 09 56 58

		function_init(2);
		function_set_arg(0);

		do_call_num(l);
		function_set_arg(1);
		function_call( &do_set_val, "do_set_val" );
		function_end(2);

	} else {

		#if Assembly 
		printf("mov %%eax,%d(%%ebp)\n", l);
		#endif
		*ind++ = 0x89;
		*ind++ =  0x85;
		*(int *)ind = l;
		ind += 4;
	}
}

function_set_arg( a ) {
	#if Assembly 
	printf("mov %%eax,%x(%%esp)\n", a * 4);
	#endif
	*ind++ = 0x89;
	*ind++ = 0x84; 
	*ind++ = 0x24;
	*(int *)ind = a * 4;
	ind += 4;
}

function_call(a,b) {
	int n = a;
	n = n - (int)ind - 5;
	#if Assembly 
	printf("call %s 0x%x\n", b, n);
	#endif
	*ind++ = 0xe8;
	while (n && n != -1) {
		*ind++ = n;
		n = n >> 8;
	}
}

function_init(a) {
	#if Assembly 
	printf("sub $%d,%%esp\n", a*4);
	#endif
	*ind++ = 0x81; 
	*ind++ = 0xec;
	*(int *)ind =  a * 4; 
	ind += 4;	
}

function_end(a) {
	#if Assembly 
	printf("add $%d,%%esp\n", a * 4);
	#endif
	*ind++ = 0x81;
	*ind++ =  0xc4;
	*(int *)ind = a * 4;
	ind += 4;
}

variable *do_fn_create_array() {

	return doarray();
}


variable *do_fn_create_object() {
	return doobject();
}

do_create_array( end ) {

/*	vars_init();

	ivar = ivar - 4;
	*(int *)indvar = -ivar;
	int l = ivar;*/

	int l = do_new_let();

	//init array
	if( end == '}' ) {
		function_init(0);
		function_call( &do_fn_create_object, "do_fn_create_object" );
		function_end(0);
	} else {

		function_init(0);
		function_call( &do_fn_create_array, "do_fn_create_array" );
		function_end(0);
	}

	do_equal(l);
	while( toks.c != end ) {

		skipLine();

		/*ivar = ivar - 4;
		*(int *)indvar = -ivar;
		int l1 = ivar;*/
		int l1 = do_new_let();

		if( toks.t == TOK_IDENT ) {
			char *id = toks.id;

			do_call_string( dostring( toks.id ) );
			next();

			if( toks.c == ',' || toks.c == '.' || toks.c == '[' || toks.c == ']' || toks.c == '}' ) {
		 		int l = array_get1( &var_stk, id );
			 	do_call_var(l);
				do_after_ident();		 	
			}


		} else {
			expr();
		}

		do_equal(l1);

		/*ivar = ivar - 4;
		*(int *)indvar = -ivar;
		int l2 = ivar;*/
		int l2 = do_new_let();

		int d = 0;
		if( toks.c == ':' ) {
			d = 1;
			next();
			expr();
		}
		do_equal(l2);

		function_init(3);

		do_call_var(l);
		function_set_arg(0);

		do_call_var(l1);
		function_set_arg(1);

		skipLine();
		if( d ) {
			//next();

			//expr();
			do_call_var(l2);

			function_set_arg(2);

			function_call( &array_set, "array_set" );

			function_end(3);
		} else {

			do_call_num(0);
			function_set_arg(2);

			function_call( &array_set2, "array_set2" );
			function_end(3);			
		}

		skipLine();
		if( toks.c == ',' ) {
			next();
		}
		skipLine();
	}

	do_call_var(l);

	skip(end);
}

do_call_array() {

	while( toks.c == '[' ) {

		/*vars_init();
		ivar = ivar - 4;
		*(int *)indvar = -ivar;
		int l1 = ivar;*/
		int l1 = do_new_let();
		do_equal(l1);

		next();
		expr();

		/*ivar = ivar - 4;
		*(int *)indvar = -ivar;
		int l2 = ivar;*/
		int l2 = do_new_let();
		do_equal(l2);


		//do_call( &array_get, "array_get", l1, l2 );

		function_init(2);

		do_call_var(l1);
		function_set_arg(0);

		
		do_call_var(l2);
		function_set_arg(1);

		function_call( &array_get, "array_get" );

		function_end(2);
		skip(']');
	}

/*	if( toks.c == '.' ) {
		while( toks.c == '.' ) {
			skip('.');
			do_call_object(&toks);
		}
	}*/

	if( toks.t == 2048 ) {

		next();

		/*vars_init();
		ivar = ivar - 4;
		*(int *)indvar = -ivar;
		int l1 = ivar;*/
		int l1 = do_new_let();

		do_equal(l1);

		expr();

		/*ivar = ivar - 4;
		*(int *)indvar = -ivar;
		int l2 = ivar;*/
		int l2 = do_new_let();

		do_equal(l2);



		function_init(2);
		do_call_var(l1);
		function_set_arg(0);

		do_call_var(l2);
		function_set_arg(1);
		function_call( &array_set_val, "array_set_val" );

		function_end(2);

	}
}


do_call_object( tokens *ctoks ) {

	if( strcmp( ctoks->id, "val" ) == 0 ) {
		do_get_val();
	} else if( strcmp( ctoks->id, "type" ) == 0 ) {
		do_get_val();					
	} else {

		function_init(2);
		function_set_arg(0);
		variable *var = safe_alloc_new(&alloc, sizeof(variable));
		var->val = ctoks->id;
		var->type = 1;

		do_call_string( var );
		function_set_arg(1);
		function_call( &array_get, "array_get" );
		function_end(2);

		if( toks.t == 2048 ) {
			next();

			/*vars_init();
			ivar = ivar - 4;
			*(int *)indvar = -ivar;
			int l1 = ivar;*/
			int l1 = do_new_let();
			do_equal(l1);

			expr();

			/*ivar = ivar - 4;
			*(int *)indvar = -ivar;
			int l2 = ivar;*/
			int l2 = do_new_let();
			do_equal(l2);

			function_init(2);
			do_call_var(l1);
			function_set_arg(0);

			do_call_var(l2);
			function_set_arg(1);
			function_call( &array_set_val, "array_set_val" );
			function_end(2);
		}
	}			
}

do_call_address( l ) {
			
	#if Assembly 
	printf("leal %d(%%ebp),%%eax\n", l);
	#endif
	*ind++ = 0x8d;
	*ind++ = 0x85;
	*(int *)ind = l;
	ind += 4;

}

do_is_not_end() {
	return toks.c != ';' && toks.t != 2022 && toks.t != 2023 && toks.t != TOK_IN && toks.c != ')' && toks.c != '?';
}

do_create_var( n ) {
	int i = 0;
	while( do_is_not_end() ) {

		variable *a = safe_alloc_new( &alloc, sizeof(variable) );
		char *id = toks.id;

		array_set1( &var_stk, id, a );


		tokens btoks;
		btoks.t = toks.t;
		btoks.id = toks.id;
		btoks.c = toks.c;

		vtoks[i] = safe_alloc_new( &alloc, sizeof( tokens ) );
		
		memcpy( vtoks[i], &btoks, sizeof( tokens ) );

		i++;

		next();
		if( toks.c == ',' )
			skip(',');

		if( toks.t == 2048 ) {
			do_main_equal(&btoks);

		}
	}
}

do_create_let( n ) {
	vars_init();
	int i = 0;
	while( do_is_not_end() ) {

		ivar = ivar - n;
		*(int *)indvar = -ivar;
		char *id = toks.id;
		/*if( thisClass ) {
			id = mstrcat( thisClass, "%");
			id = mstrcat(id, toks.id);
		}*/


		array_set1( &var_stk, id, ivar );
		array_set1( &let_stk, id, ivar );


		tokens btoks;
		btoks.t = toks.t;
		btoks.id = toks.id;
		btoks.c = toks.c;

		vtoks[i] = safe_alloc_new( &alloc, sizeof( tokens ) );
		
		memcpy( vtoks[i], &btoks, sizeof( tokens ) );

		i++;

		next();
		if( toks.c == ',' )
			skip(',');

		if( toks.t == 2048 ) {
			do_main_equal(&btoks);
/*			next();
			char *id = btoks.id;
			int l = array_get1( &var_stk, id );

			expr();

print_tok();
exit(0);

			do_equal( l );*/

		}
	}
}

do_main_equal( tokens *btoks ) {
	next();

	char *id = btoks->id;
	int l = array_get1( &var_stk, id);
	expr();

	do_equal( l );	
}

do_new_class( cls ) {
	vars_init();
	
	int l = array_get1( &var_stk, cls );
/*	printf("%d\n", l);
	exit(0);*/

	function_init(1);
	if( ! l ) {
		array_set_int( &ind_cls, ind+1, cls );
	}

	do_call_num(l);

	function_set_arg(0);
	function_call( &do_fn_new_class, "do_fn_new_class" );
	function_end(1);


	/*ivar = ivar - 4;
	*(int *)indvar = -ivar;
	int l1 = ivar;*/
	int l1 = do_new_let();
	do_equal(l1);

	if( toks.c == '(' ) {

		function_init(2);
		function_set_arg(0);

		do_call_num( dostring( "prototype" ) );
		function_set_arg(1);
		function_call( &array_get, "array_get" );
		function_end(2);

		function_init(2);
		function_set_arg(0);

		do_call_num( dostring( "construct" ) );
		function_set_arg(1);
		function_call( &array_get, "array_get" );
		function_end(2);


		/*ivar = ivar - 4;
		*(int *)indvar = -ivar;
		int ld = ivar;*/
		int ld = do_new_let();
		do_equal(ld);

		do_call_var( l1 );
		do_call_function_callback(ld);

	}

	do_call_var(l1);

}

//do_call_class( cls ) {	
do_create_main_class( cls ) {	

	//create object for class
	function_init(0);
	function_call( &do_fn_create_object, "do_fn_create_object" );
	function_end(0);

	/*vars_init();
	ivar = ivar - 4;
	*(int *)indvar = -ivar;
	int l = ivar;*/
	int l = do_new_let();
	do_equal(l);

	//do_call_var(l);
	//set class name and all methods address in object
	char *t;
	t = mstrcat(cls, "%fn%");

	function_init(3);
	function_set_arg(0);

	do_call_num( dostring( "class" ) );
	function_set_arg(1);

	do_call_num( dostring( cls ) );
	function_set_arg(2);
	function_call( &array_set, "array_set" );
	function_end(3);

	do_call_var(l);
	function_init(3);
	function_set_arg(0);

	do_call_num( dostring( "prototype" ) );
	function_set_arg(1);

/*	array *arr1 = safe_alloc_new( &alloc, sizeof( array ) );
	array_init( arr1 );*/
	variable * arr = doobject( );
	//variable * b = dostring( cls );
	do_call_num(arr);
	function_set_arg(2);
	function_call( &array_set, "array_set" );
	function_end(3);

	for( int i = 0; i < sym_stk.length ; i++ ) {
		int p = strstr(sym_stk.key[i], t );
		if( p ) {
			int ll = sym_stk.value[i];

			int len = strlen(cls);
			char *v = safe_alloc_new(&alloc, sizeof( char *) );
			v = sym_stk.key[i];
			v += len+4;

			do_call_num(arr);

			function_init(3);
			function_set_arg(0);

			do_call_num( dostring( v ) );
			function_set_arg(1);
			variable * b1 = donvar( ll, 4 );
			do_call_num(b1);
			function_set_arg(2);
			function_call( &array_set, "array_set" );
			function_end(3);

		}
	}

	do_call_var(l);
}

do_create_class() {
	next();
	char *cls = toks.id;
	thisClass = cls;
	toks.type = 3;
	array_set1(&cls_stk, cls, 1 );

	variable *a = safe_alloc_new( &alloc, sizeof(variable ) );
	a->type = DOTYPE_OBJECT;
	array_set1(&var_stk, cls, a );


	set_tokv( &toks, cls, 0 );
	next();

	if( toks.t == TOK_EXTENDS ) {
		//printf("ddddddddd\n");
		//exit(0);

		next();
		char *t;
		t = mstrcat( toks.id, "%fn%");

		for( int i = 0; i < sym_stk.length ; i++ ) {
			int p = strstr(sym_stk.key[i], t );

			if( p ) {

				int len = strlen( toks.id );
				char *d = safe_alloc_new( &alloc, sizeof( char *) );
				d = sym_stk.key[i];
				d += len+4;

				char *t1;
				t1 = mstrcat( cls, "%fn%");
				t1 = mstrcat( t1, d);

				int ll = sym_stk.value[i];
				//printf("%s\n", sym_stk.key[i]);

				array_set1( &sym_stk, t1, ll);
			}
		}

		next();

	}

	skip('{');
	if( strcmp(file_ext, "js") == 0 ) {
		decl_js( cls );

	} else {
		decl(cls);
	}


	skip('}');

	return cls;
}


do_create_callback_function() {
	#if Assembly 
	printf("jmp  $\n");
	#endif
	*ind++ = 0xe9;
	*(int *)ind = 0;
	int b = ind;
	ind += 4;

	do_main_create_function("","");

	*(int *)b = ind - b - 4;

	variable * a = donvar( b+4, 4 );
	do_call_num(a);
}

do_main_create_function( cls, fn_name ) {
	skip('(');
	int a = 8;


	array_set1(&var_stk, "this", a );
	a += 4;

	while( toks.c != ')' ) {
		if( toks.t == 1 | toks.t == 10 ) {
			next();
		}

		array_set1(&var_stk, toks.id, a );
		a += 4;

		next();
		if (toks.c == ',')
			next();
	}
	skip(')');		
	#if Assembly 
	printf("------------------ function %s : \n", fn_name);
	#endif

	#if Assembly 
	printf("push %%ebp\n");
	#endif
	*ind++ = 0x55;

	#if Assembly
	printf("mov %%esp,%%ebp\n");
	#endif					
	*ind++ = 0x89;
	*ind++ = 0xe5;

	int bivar = ivar;
	int bindvar = indvar;
	//ivar = 0;
	//do_new_let();

	if( strcmp(fn_name, "main") == 0 ) {
		//create class object
		for( int i = 0; i < cls_stk.length; i++ ) {
			char *cls1 = cls_stk.key[i];
			//printf("%s\n", cls1);
			//exit(0);

			int a = array_get1( &var_stk, cls1);

			do_create_main_class( cls1 );
			do_equal(a);
		}

	}

	block();

	#if Assembly 
	printf("leave\n");
	#endif
	*ind++ = 0xc9;
	#if Assembly 
	printf("ret\n");
	#endif
	*ind++ = 0xc3;

	//ivar = bivar;
	//indvar = bindvar;
}

do_create_main_function( cls ) {

	#if Assembly 
	printf("push %%ebp\n");
	#endif
	*ind++ = 0x55;

	#if Assembly 
	printf("mov %%esp,%%ebp\n");
	#endif					
	*ind++ = 0x89;
	*ind++ = 0xe5;

	while( ch != EOF ) {
		block();
	}

	#if Assembly 
	printf("leave\n");
	#endif
	*ind++ = 0xc9;
	#if Assembly 
	printf("ret\n");
	#endif
	*ind++ = 0xc3;
}

do_create_function( cls ) {
	char *fn_name = toks.id;
	next();

	do_main_create_function( cls, fn_name );
}

do_plus_plus() {
	//do_call_var(l);

	//should convert to assembly ...
	function_init(1);
	function_set_arg(0);
	function_call( &do_fn_plus_plus, "do_fn_plus_plus" );
	function_end(1);
}

do_and() {
	#if Assembly 
	printf("and %%ecx,%%eax\n");
	#endif
	*ind++ = 0x21;
	*ind++ = 0xc8;
}

do_or() {
	#if Assembly 
	printf("or %%ecx,%%eax\n");
	#endif
	*ind++ = 0x09;
	*ind++ = 0xc8;
}

do_or_or(a) {

	#if Assembly 
	printf("mov  0x4(%%eax),%%eax\n");
	#endif
	*ind++ = 0x8b;
	*ind++ = 0x40;
	*ind++ = 0x04;


	#if Assembly 
	printf("test %%eax,%%eax\n");
	#endif
	*ind++ = 0x85;
	*ind++ = 0xc0;

	#if Assembly 
	printf("jne $\n");
	#endif
	*ind++ = 0x0f;
	*ind++ = 0x85;

	*(int *)ind = a;
	a = ind;
	ind += 4;
	return a;
}

do_patch_or_or(a) {
	#if Assembly
	printf("mov $0x0,%%eax\n");
	#endif
	*ind++ = 0xb8;
	ind += 4;

	do_convert_to_var(2);

	#if Assembly
	printf("jmp 0x5c\n");
	#endif
	*ind++ = 0xe9;
	*(int *)ind = 0;
	int s = ind;
	ind += 4;

    int n;
    while (a) {
        n = *(int *)a;
        *(int *)a = ind - a - 4;
        a = n;
    }
	
	#if Assembly
	printf("mov $0x1,%%eax\n");
	#endif
	*ind++ = 0xb8;
	*ind = 1;
	ind += 4;

	do_convert_to_var(2);
	*(int *)s = ind - s - 4;	
}

do_and_and(a) {
	#if Assembly 
	printf("mov  0x4(%%eax),%%eax\n");
	#endif
	*ind++ = 0x8b;
	*ind++ = 0x40;
	*ind++ = 0x04;


	#if Assembly 
	printf("test %%eax,%%eax\n");
	#endif
	*ind++ = 0x85;
	*ind++ = 0xc0;

	#if Assembly 
	printf("je $\n");
	#endif
	*ind++ = 0x0f;
	*ind++ = 0x84;
	*(int *)ind = a;
	a = ind;
	ind += 4;
	return a;
}

do_patch_and_and(a) {
	#if Assembly
	printf("mov 1,%%eax\n");
	#endif
	*ind++ = 0xb8;
	*ind = 1;
	ind += 4;

	do_convert_to_var(2);

	#if Assembly
	printf("jmp 0x5c\n");
	#endif
	*ind++ = 0xe9;
	*(int *)ind = 0;
	int s = ind;
	ind += 4;

    int n;
    while (a) {
        n = *(int *)a;
        *(int *)a = ind - a - 4;
        a = n;
    }

	/**(int *)a = ind - a - 4;
	*(int *)d = ind - d - 4;
	*/
	#if Assembly
	printf("mov 0,%%eax\n");
	#endif
	*ind++ = 0xb8;
	*ind = 0;
	ind += 4;

	do_convert_to_var(2);
	*(int *)s = ind - s - 4;	
}



do_for_loop() {

	skip(';');
	int n = ind;
	test_expr();
	skip(';');


	#if Assembly 
	printf("jmp  $\n");
	#endif
	*ind++ = 0xe9;
	*(int *)ind = 0;
	ind += 4;

	int t = ind - 4;
	expr();

	#if Assembly 
	printf("jmp  $\n");
	#endif
	*ind++ = 0xe9;
	*(int *)ind = n - (int)ind - 4;
	ind += 4;
	*(int *)t = (int)ind - t - 4;
	n = t + 4;
	skip(')');

	#if Assembly 
	printf("mov  0x4(%%eax),%%eax\n");
	#endif
	*ind++ = 0x8b;
	*ind++ = 0x40;
	*ind++ = 0x04;

	#if Assembly 
	printf("test %%eax,%%eax\n");
	#endif
	*ind++ = 0x85;
	*ind++ = 0xc0;

	#if Assembly 
	printf("je $\n");
	#endif
	*ind++ = 0x0f; 
	*ind++ = 0x84; 
	*(int *)ind = 0;
	ind += 4;


	int b = ind - 4;

	block();

	*(int *)b = (int)ind - b + 1;

	n = n - (int)ind - 5;
	#if Assembly 
	printf("jmp  $\n");
	#endif
	*ind++ = 0xe9;
	*(int *)ind = n;
	ind += 4;
}



do_shift_left() {

	function_init(2);
	function_set_arg(0);

	#if Assembly 
	printf("mov %%ecx, %%eax\n");
	#endif
	*ind++ = 0x89;
	*ind++ = 0xc8;

	function_set_arg(1);
	function_call( &do_fn_shift_left, "do_fn_shift_left" );
	function_end(2);

}

do_shift_right() {
	function_init(2);
	function_set_arg(0);

	#if Assembly 
	printf("mov %%ecx, %%eax\n");
	#endif
	*ind++ = 0x89;
	*ind++ = 0xc8;

	function_set_arg(1);
	function_call( &do_fn_shift_right, "do_fn_shift_right" );
	function_end(2);
}

do_less_equal() {

}

do_greater_than() {

}


do_greater_equal() {

}

do_not() {

	#if Assembly 
	printf("mov  0x4(%%eax),%%eax\n");
	#endif
	*ind++ = 0x8b;
	*ind++ = 0x40;
	*ind++ = 0x04;

	#if Assembly
	printf("mov 0x0,%%ecx\n");
	#endif
	*ind++ = 0xb9;
	ind += 4;

	#if Assembly
	printf("cmp %%eax,%%ecx\n");
	#endif
	*ind++ = 0x39;
	*ind++ = 0xc1;


	#if Assembly
	printf("mov 0x0,%%eax\n");
	#endif
	*ind++ = 0xb8;
	ind += 4;

	#if Assembly
	printf("sete al\n");
	#endif
	*ind++ = 0x0f;
	*ind++ = 0x94;
	*ind++ = 0xc0;

	do_convert_to_var();

}

do_plus_equal() {

/*	vars_init();

	ivar = ivar - 4;
	*(int *)indvar = -ivar;
	int l1 = ivar;*/
	int l1 = do_new_let();

	do_equal(l1);
	next();

	/*ivar = ivar - 4;
	*(int *)indvar = -ivar;
	int l2 = ivar;*/
	int l2 = do_new_let();
	expr();
	do_equal(l2);

	function_init(2);
	do_call_var(l2);
	function_set_arg(0);

	do_call_var(l1);
	function_set_arg(1);
	function_call( &do_fn_add, "do_fn_add" );
	function_end(2);

	//do_equal(l);
}

do_for_in() {

/*	vars_init();

	ivar = ivar - 4;
	*(int *)indvar = -ivar;
	int l = ivar;*/
	int l = do_new_let();

	do_call_var(l);

	do_call_num(0);
	do_convert_to_var(2);
	do_equal(l);

	int n = ind;
	do_call_var(l);
	do_push();

	function_init(1);
	expr();
/*	ivar = ivar - 4;
	*(int *)indvar = -ivar;
	int l1 = ivar;*/
	int l1 = do_new_let();

	//do_call_var(l1);
	do_equal(l1);
	function_set_arg(0);
	function_call( &array_len, "array_len" );
	function_end(1);

	do_pop();

	do_get_var_value();

	do_less_than();
	skip(')');

	#if Assembly 
	printf("test %%eax,%%eax\n");
	#endif
	*ind++ = 0x85;
	*ind++ = 0xc0;

	#if Assembly 
	printf("je $\n");
	#endif
	*ind++ = 0x0f; 
	*ind++ = 0x84; 
	*(int *)ind = 0;
	ind += 4;
	int b = ind - 4;

	//vtoks[0]->type = 2;
	int k = array_get1( &var_stk, vtoks[0]->id );//get_tokv( vtoks[0], 0 );

	function_init(2);
	do_call_var(l1);
	function_set_arg(0);
	do_call_var(l);
	function_set_arg(1);
	function_call( &array_key, "array_key" );
	function_end(2);

	do_equal(k);

	//vtoks[1]->type = 2;
	int v = array_get1( &var_stk, vtoks[1]->id );//get_tokv( vtoks[1], 0 );

	function_init(2);
	do_call_var(l1);
	function_set_arg(0);
	do_call_var(l);
	function_set_arg(1);
	function_call( &array_value, "array_value" );
	function_end(2);

	do_equal(v);


	block();
	
	do_call_var(l);
	do_plus_plus();

	*(int *)b = (int)ind - b + 1;

	n = n - (int)ind - 5;
	#if Assembly 
	printf("jmp  $\n");
	#endif
	*ind++ = 0xe9;
	*(int *)ind = n;
	ind += 4;
}