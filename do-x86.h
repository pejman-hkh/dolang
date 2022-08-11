skip(a);
next();
block();
o();
print_tok();
char * mstrcat();
void mprint( variable *a );

#include "fn.h"
#include "conf.h"

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

	if( l > 0 & l > 10000 ) {
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

do_minus_minus(l) {
	do_call_var(l);
	
	#if Assembly 
	printf("addl $0xffffffff,%d(%%ebp)\n", l-4);
	#endif
	*ind++ = 0x83;
	*ind++ = 0x85;
	*(int *)ind = l-4;
	ind += 4;	
	*ind++ = -1;
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

	function_call( &do_fn_equal_equal, "do_fn_equal_equal" );
	function_end(2);

}

do_not_equal() {
	#if Assembly 
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
	*ind++ = 0xc0;

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

	if( l > 0 & l > 10000 ) {
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

variable *do_fn_create_array() {
	array *arr = safe_alloc_new(&alloc, sizeof(array));
	array_init( arr );

	/*variable *var = safe_alloc_new(&alloc, sizeof(variable));
	var->val = arr;
	var->type = 3;
*/
	dovar( var, arr, 3 );

	return var;
}

do_create_array( end ) {
	vars_init();

	ivar = ivar - 4;
	*(int *)indvar = -ivar;
	int l = ivar;


	//init array
	function_init(0);
	function_call( &do_fn_create_array, "do_fn_create_array" );
	function_end(0);
	do_equal(l);

	while( toks.c != end ) {

		function_init(3);

		do_call_var(l);

		function_set_arg(0);

		if( toks.t == TOK_IDENT ) {
	
			dovar(a,toks.id,1);
			do_call_string(a);
			next();

		} else {
			expr();
		}

		function_set_arg(1);
		int ntype = type;

		if( toks.c == ':' ) {
			next();

			expr();
			function_set_arg(2);

			function_call( &array_set, "array_set" );

			function_end(3);
		}


		if( toks.c == ',' ) {
			next();
		}
	}

	do_call_var(l);

	skip(end);
}

do_call_array(l) {

	while( toks.c == '[' ) {
		function_init(2);

		function_set_arg(0);

		next();
		expr();	
		function_set_arg(1);

		function_call( &array_get, "array_get" );

		function_end(2);
		skip(']');
	}

	if( toks.c == '.' ) {
		while( toks.c == '.' ) {
			skip('.');
			do_call_object(&toks);
		}
	}

	if( toks.c == '=' ) {

		next();


		function_init(2);
		function_set_arg(0);

		expr();

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

		if( toks.c == '=' ) {
			next();

			function_init(2);
			function_set_arg(0);

			expr();

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

do_create_var( n ) {
	vars_init();


	int i = 0;
	while( toks.c != ';' && toks.t != 2022 && toks.t != 2023 && toks.t != TOK_IN ) {

		ivar = ivar - n;
		char *id = toks.id;
		/*if( thisClass ) {
			id = mstrcat( thisClass, "%");
			id = mstrcat(id, toks.id);
		}*/


		array_set1( &var_stk, id, ivar );

		*(int *)indvar = -ivar;

		tokens btoks;
		btoks.t = toks.t;
		btoks.id = toks.id;
		btoks.c = toks.c;

		vtoks[i] = malloc( sizeof( tokens ) );
		
		memcpy( vtoks[i], &btoks, sizeof( tokens ) );

		i++;

		next();
		if( toks.c == ',' )
			skip(',');

		if( toks.c == '=' ) {
			next();
			char *id = btoks.id;
			int l = array_get1( &var_stk, id );

			expr();
			do_equal( l );

		}
	}
}

do_call_class( cls ) {	

	//create object for class
	function_init(0);
	function_call( &do_fn_create_array, "do_fn_create_array" );
	function_end(0);

	vars_init();
	ivar = ivar - 4;
	*(int *)indvar = -ivar;
	int l = ivar;

	do_equal(l);

	//do_call_var(l);
	//set class name and all methods address in object
	char *t;
	t = mstrcat(cls, "%fn%");

	function_init(3);
	function_set_arg(0);
	dovar(a,"class",1);
	do_call_num(a);
	function_set_arg(1);
	dovar(b,cls,1);
	do_call_num(b);
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

			do_call_var(l);

			function_init(3);
			function_set_arg(0);
			dovar(a1,v,1);
			do_call_num(a1);
			function_set_arg(1);
			dovar(b1,ll,4);
			do_call_num(b1);
			function_set_arg(2);
			function_call( &array_set, "array_set" );
			function_end(3);

		}
	}

	do_call_var(l);

	if( toks.c == '(' ) {
	
		char *t;
		t = mstrcat( cls, "%fn%");
		t = mstrcat( t, "construct");

		int l1 = array_get1( &sym_stk, t );
		dovar(a,l1,4);
		do_call_function_callback( a );
	}

	do_call_var(l);
}

do_create_class() {
	next();
	char *cls = toks.id;
	toks.type = 3;
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
	decl(cls);
	skip('}');
}


do_create_callback_function() {
	#if Assembly 
	printf("jmp  $\n");
	#endif
	*ind++ = 0xe9;
	*(int *)ind = 0;
	int b = ind;
	ind += 4;

	do_main_create_function();

	*(int *)b = ind - b - 4;

	dovar(a, b+4, 4);
	do_call_num(a);
}

do_main_create_function( cls ) {
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
	printf("push %%ebp\n");
	#endif
	*ind++ = 0x55;

	#if Assembly 
	printf("mov %%esp,%%ebp\n");
	#endif					
	*ind++ = 0x89;
	*ind++ = 0xe5;

	block();

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

	do_main_create_function( cls );
}

do_plus_plus(l) {
	do_call_var(l);

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
	int indp = ind;
	*(int *)ind = 0;
	ind += 4;
	return indp;
}

do_patch_or_or(a,d) {
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

	*(int *)a = ind - a - 4;
	*(int *)d = ind - d - 4;
	
	#if Assembly
	printf("mov $0x1,%%eax\n");
	#endif
	*ind++ = 0xb8;
	*ind = 1;
	ind += 4;

	do_convert_to_var(2);
	*(int *)s = ind - s - 4;	
}

do_and_and() {

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

}

do_shift_right() {

}

do_less_equal() {

}

do_greater_than() {

}


do_greater_equal() {

}

do_not() {
	
}


do_for_in() {

	ivar = ivar - 4;
	*(int *)indvar = -ivar;
	int l = ivar;

	do_call_var(l);

	do_call_num(0);
	do_convert_to_var(2);
	do_equal(l);

	int n = ind;
	do_call_var(l);
	do_push();

	function_init(1);
	expr();
	ivar = ivar - 4;
	*(int *)indvar = -ivar;
	int l1 = ivar;

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
	do_plus_plus(l);

	*(int *)b = (int)ind - b + 1;

	n = n - (int)ind - 5;
	#if Assembly 
	printf("jmp  $\n");
	#endif
	*ind++ = 0xe9;
	*(int *)ind = n;
	ind += 4;
}