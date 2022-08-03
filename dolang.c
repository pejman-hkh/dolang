#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/mman.h>
#include <dlfcn.h>

typedef struct {
	char type;
	void * val;
} variable;

#define dovar(a,b,c) \
variable *a = malloc( sizeof( variable *) );\
a->val = b;\
a->type = c;


#define TOK_IF 2
#define TOK_ELSE 3
#define TOK_WHILE 4
#define TOK_BREAK 5
#define TOK_RETURN 6
#define TOK_FOR 7
#define TOK_DEFINE 8
#define TOK_MAIN 9
#define TOK_VAR 10
#define TOK_NEW 11
#define TOK_CLASS 12
#define TOK_IN 15
#define TOK_THIS 16
#define TOK_FUNC 17
#define TOK_LET 18


#define TOK_IDENT 999
#define TOK_STRING 1000

#include "safe_alloc.h"
safe_alloc alloc;
#include "array.h"
#define ALLOC_SIZE 99999999

char ch;
FILE *file;
char *buf;
char *sbuf;
//int vars;

char *prog;
char *ind;
char *ind_main;


int indvar;
int ivar;
int type;
array ext;

char *thisClass;

//int sym_stk, dstk;

typedef struct
{
	int t;
	char c;
	int l;
	int id;
	char type;
	char *class;
} tokens ;

tokens toks;
array sym_stk;
array var_stk;
//array var_type;
//array var_ref;
tokens ** vtoks;


int inMain = 0;

#include "do-x86.h";
#include "ext.h";

inp() {
	ch = fgetc( file );
}


isid()
{
	return isalnum(ch) | ch == '_';
}

getq()
{
	if (ch == '\\') {
		inp();
		if (ch == 'n')
			ch = '\n';
	}
}


skip( char s ) {
	if( toks.c == s ) {
		next();
	} else {
		printf("Expected %c \n", s );
		exit(0);
	}
}

o(n)
{
	while (n && n != -1) {
		*(char *)ind++ = n;
		n = n >> 8;
	}
}

char * mstrcat( char *a, char *b) {
	char *r;
	r = safe_alloc_new(&alloc, strlen(a)+strlen(b)+1);
	while( *a ) {
		*(char *)r++ = *a++;
	}

	while( *b ) {
		*(char *)r++ = *b++;
	}

	*r = '\0';

	return safe_alloc_get(&alloc);
}

tokv_id( tokens *tok1, char *cls ) {
	char *id = tok1->id;
	if( cls != 0 & tok1->type == 1 ) {
		char *t;
		t = mstrcat(cls, "%fn%");
		id = mstrcat(t, id);
	} else if( tok1->type == 1 ) {
		id = mstrcat("fn%", id);	
	} else if( tok1->type == 2 ) {
		id = mstrcat("var%", id);
	} else if( tok1->type == 3 ) {
		id = mstrcat("object%", id);
	}

	return id;	
}

set_tokv( tokens *tok1, int val, char *cls ) {
	char *id = tokv_id( tok1, cls );
	array_set1( &sym_stk, id, val );
}

get_tokv( tokens *tok1, char *cls ) {
	char *id = tokv_id( tok1, cls );
	return array_get1( &sym_stk, id );
}

array mt;

next() {
	toks.t = 0;
	toks.c = 0;
	toks.l = 0;
	toks.id = 0;

	if( isspace(ch) ) {
		while( isspace( ch ) ) {
			inp();
		}
	}

	if( ch == '#' ) {
		toks.t = 1003;
		inp();
		toks.id = buf;
		while( ch != '\n' ) {
			*buf++ = ch;
			inp();
		}
		*buf++ = 0;

	} else if( isid() ) {

		toks.id = buf;
		toks.c = ch;
		while( isid() ) {
			*(char *)buf++ = ch;
			inp();
		}
		*(char *)buf++ = 0;

		int a = array_get1( &mt, toks.id );
		if( isdigit( toks.c ) ) {
			toks.t = 1005;
		} else if( a != 0 ) {
			toks.t = a;
		} else {
			toks.t = TOK_IDENT;
		}

	} else {

		if( ch == '[' ) {
			toks.t = 1007;
			toks.c = ch;
			inp();
		} else if( ch == '\'' ) {
			toks.t = TOK_STRING;
			inp();
			toks.id = buf;
			while( ch != '\'' ) {
				getq();
				*buf++ = ch;
				inp();
			}
			*buf++ = 0;
			inp();
		} else if( ch == '"' ) {
			toks.t = TOK_STRING;
			inp();
			toks.id = buf;
			while( ch != '"' ) {
				getq();
				*buf++ = ch;
				inp();
			}
			*buf++ = 0;
			inp();

		
		}  else {
			toks.t = 1001;
			toks.c = ch;
			inp();

			if( toks.c == '*' ) {
				toks.l = 1;
				toks.t = 2000;
			} else if( toks.c == '/' ) {
				toks.l = 1;
				toks.t = 2021;
				if( ch == '/' ) {
					toks.l == 11;
					toks.t = 1006;

					while( ch != '\n' ) {
						inp();
					}

					inp();
															
				} else if( ch == '*' ) {
					toks.l == 11;
					toks.t = 1006;
					while( 1 ) {
						int ch1 = ch;
						inp();
						if( ch == '/' & ch1 == '*' ) {
							break;
						}
					}
					inp();
				}
				
			} else if( toks.c == '%' ) {
				toks.l = 1;
				toks.t = 2001;
			} else if( toks.c == '+' ) {
				toks.l = 2;
				toks.t = 2002;
				if( ch == '+' ) {
					toks.l = 11;
					toks.t = 2003;
					toks.c = ch;
					inp();
				}
			} else if( toks.c == '-' ) {
				toks.l = 2;
				toks.t = 2004;

				if( ch == '-' ) {
					toks.l = 11;
					toks.t = 2005;
					toks.c = ch;
					inp();		 
				}

			} else if( toks.c == '<' ) {
				toks.l = 4;
				toks.t = 2006;

				if( ch == '<' ) {
					toks.l = 3;
					toks.t = 2007;
					toks.c = ch;
					inp();
				} else if( ch == '=' ) {
					toks.l = 4;
					toks.t = 2008;
					toks.c = ch;
					inp();
				}
			} else if( toks.c == '>' ) {
				toks.l = 4;
				toks.c = 0xf;
				toks.t = 2009;

				if( ch == '>' ) {
					toks.l = 3;
					toks.t = 2010;
					toks.c = ch;
					inp();		  
				} else if( ch == '=' ) {
					toks.l = 4;
					toks.t = 2011;
					toks.c = ch;
					inp();		  
				}
			} else if( toks.c == '!' ) {

				toks.l = 2;
			   	toks.t = 2012;

				if( ch == '=' ) {
					toks.l = 5;
					toks.t = 2013;
					inp();

				}
			} else if( toks.c == '=' ) {
				if( ch == '=' ) {
					toks.l = 5;
					toks.t = 2014;
					toks.t = ch;
					inp();
				}
			} else if( toks.c == '&' ) {
				toks.l = 6;
				toks.t = 2015;
				if( ch == '&' ) {
					toks.l = 9;
					toks.t = 2016;
					toks.c = ch;
					inp();
				}	   
			} else if( toks.c == '^' ) {
				toks.l = 7;
				toks.t = 2017;

			} else if( toks.c == '|' ) {
				toks.l = 8;
				toks.t = 2018;

				if( ch == '|' ) {
					toks.l = 10;
					toks.t = 2019;
					toks.c = ch;
					inp();
				}
			} else if( toks.c == '~' ) {
				toks.l = 2;
				toks.t = 2020;
			}
		}
	}

}

unary() {

	if( toks.t == TOK_FUNC ) {
		next();
		if( toks.c == '(') {
			do_create_callback_function();
		} else {		
			toks.type = 1;
			set_tokv( &toks, ind + 5, 0 );
			next();
			do_create_callback_function();
		}

	} else if( toks.t == TOK_CLASS ) {
		do_create_class();
	} else if( toks.t == TOK_STRING ) {
		//type = 1;
		dovar( var, toks.id, 1 );
	/*	
		variable *var = safe_alloc_new(&alloc, sizeof(variable));
		var->val = toks.id;
		var->type = 1;
*/
		do_call_string( var );

		next();
		if( toks.c == '.' ) {
			//call string functions or variables
			do_get_val();
			next();
			next();
		}

	} else {


		tokens btoks;
		btoks.t = toks.t;
		btoks.id = toks.id;
		btoks.c = toks.c;

		next();

		if( btoks.c == '{' ) {
			do_create_array('}');			
		} else if( btoks.c == '[' ) {
			do_create_array(']');

		} else if( btoks.t == TOK_FUNC ) {
			do_create_callback_function();
		} else if( btoks.t == 1 | btoks.t == TOK_VAR  ) {
			if( toks.c == '*' ) {
				next();
			}

			do_create_var( 4 );
	
		} else if( btoks.c == '&' ) {
	
			int l = array_get1( &var_stk, toks.id);
			do_call_address( l );

			next();

		} else if( toks.t == TOK_STRING & btoks.c == '+' ) {
			/*printf("ddddddddddd\n");
			exit(0);*/

		} else if( btoks.t == TOK_NEW ) {

			do_call_class( &btoks, &toks );
		} else if( toks.t == 2003 ) {
			//btoks.type = 2;
			//int l = get_tokv( &btoks, 0 );
			int l = array_get1( &var_stk, btoks.id);
			do_plus_plus( l );
			next();		
		} else if( toks.t == 2005 ) {
			//btoks.type = 2;
			//int l = get_tokv( &btoks, 0 );
			int l = array_get1( &var_stk, btoks.id);
			do_minus_minus( l );
			next();
		} else if( btoks.t == 1005 ) {
		/*	type = 2;
			variable *var = safe_alloc_new(&alloc, sizeof(variable));
			var->val = atoi(btoks.id);
			var->type = 2;*/

			do_call_num( atoi(btoks.id) );
			do_convert_to_var(2);
		} else if( toks.c == '=' & toks.l == 0 ) {
			next();

			char *id = btoks.id;
			if( thisClass ) {
				id = mstrcat( thisClass, "%");
				id = mstrcat(id, btoks.id);
			}

			int l = array_get1( &var_stk, id);

			expr(l);
			do_equal( l );
		} else if( btoks.c == '(' ) {
			expr();
			skip(')');
		} else if( toks.c == '(' ) {

			int l1 = array_get1( &var_stk, btoks.id );
			if( l1 ) {
				//is function callback

				do_call_function_callback( l1 );

			} else {			
				btoks.type = 1;
				int l = get_tokv( &btoks, 0 );
				do_call_function( l, btoks.id );
			}

			//do_concat_string();
		} else if( btoks.t == TOK_IDENT  ) {

			char *id = btoks.id;
		/*	printf("%s\n", id );
			exit(0);
			*/
			int l = array_get1( &var_stk, id);
			do_call_var( l );

			if( toks.c == '[' ) {
				do_call_array(l);
			} else if( toks.c == '.') {

				int i = 0;
				while( toks.c == '.' ) {					
					skip('.');
					tokens ctoks;
					ctoks.t = toks.t;
					ctoks.id = toks.id;
					ctoks.c = toks.c;

					next();

			
					if( toks.c == '(' ) {

						char *t = ctoks.id;

			
						function_init(2);
						function_set_arg(0);
						dovar(a,t,1);
						do_call_num(a);
						function_set_arg(1);
						function_call( &array_get, "array_get" );
						function_end(2);
					
						vars_init();

						ivar = ivar - 4;
						*(int *)indvar = -ivar;
						int ld = ivar;
						
					
						do_equal(ld);

						do_call_var( l );

						do_call_function_callback(ld);

					} else {
						do_call_object(&ctoks);
					}
				}
			}			
		} else if( toks.c == ';' ) {
		} else {
			//printf("%s\n", btoks.id );
		}

	}

}

sum(l) {
	tokens btoks;
	if( l-- == 1 ) {
		unary();
	} else {

		sum(l);
		int a = 0;
	
		while( l == toks.l ) {
			btoks.t = toks.t;
			btoks.id = toks.id;
			btoks.c = toks.c;

			next();

			if( l > 8 ) {
				if( btoks.c == '|' ) {
					a = do_or_or( a );
				}
				sum(l);
			} else {

				do_push();
				sum(l);
				do_pop();
				
				if( btoks.c != '+' & btoks.c != '=' ) {
					do_get_var_value();
				}

				if( btoks.t == 2000 ) {
					do_mul();
				} else if( btoks.t == 2001 ) {
					do_remain();
				} else if( btoks.t == 2002 ) {
					do_add();
				} else if( btoks.t == 2003 ) {
				} else if( btoks.t == 2004 ) {
					do_sub();
				} else if( btoks.t == 2005 ) {
				} else if( btoks.t == 2006 ) {
					do_less_than();
				} else if( btoks.t == 2007 ) {
					do_shift_left();
				} else if( btoks.t == 2008 ) {
					do_less_equal();
				} else if( btoks.t == 2009 ) {
					do_greater_than();
				} else if( btoks.t == 2010 ) {
					do_shift_right();
				} else if( btoks.t == 2011 ) {
					do_greater_equal();
				} else if( btoks.t == 2012 ) {
					do_not();
				} else if( btoks.t == 2013 ) {
					do_not_equal();
				} else if( btoks.t == 2014 ) {
					do_equal_equal();
				} else if( btoks.t == 2015 ) {
					do_and();
				} else if( btoks.t == 2016 ) {
					do_and_and();
				} else if( btoks.t == 2017 ) {
				} else if( btoks.t == 2018 ) {
					do_or();
				} else if( btoks.t == 2019 ) {
					//do_or_or();
				} else if( btoks.t == 2020 ) {
				} else if( btoks.t == 2021 ) {
					do_div();
				}

				if( btoks.c != '+' & btoks.c != '=' ) {
					do_convert_to_var(2);
				}

			}
		}
		if (a && l > 8) {
			if( btoks.c == '|' ) {
				int b = do_or_or( a );
				printf("mov $0x0,%%eax\n");
				*ind++ = 0xb8;
				ind += 4;

				printf("jmp 0x5c\n");
				*ind++ = 0xe9;
				*ind = 0x05;
				ind += 4;

				printf("mov $0x1,%%eax\n");
				*ind++ = 0xb8;
				*ind = 1;
				ind += 4;
/*
				printf("%d\n", ind - a + 4 );
				exit(0);
*/
				*(int *)a = ind - a + 4;
				//00 00 00 00;
			}
		}
	}

}

expr() {
	sum(11);
}

test_expr() {
	sum(11);
	return ind - 4;	
}

print_ind() {
	while( ind - prog ) {
		printf("0x%02x ", *(int *)prog++ & 0xff );
	}
}

block() {

	if( toks.t == 1006 ) {
		next();
		block();
	} else if( toks.t == TOK_FOR ) {
		next();
		skip('(');

		expr();
		
		if( toks.id && strcmp( toks.id, "in" ) == 0 ) {

			next();
			do_for_in();
		} else {

			do_for_loop();								
		}

	} else if( toks.t == TOK_WHILE ) {
		next();
		skip('(');

		
		int n = ind;
		test_expr();

		skip(')');

		do_while_loop(n);

	} else if( toks.t == TOK_IF ) {
	
		next();

		skip('(');
		test_expr();
		skip(')');		
		do_if_cond();
		int a = ind - 4;

		block();

	
		if( toks.t == TOK_ELSE ) {
			do_else_cond();
			int n = ind - 4;

			*(int *)a = (int)ind - a - 4;
		
			block();
			*(int *)n = (int)ind - n - 4;	
		} else {
			*(int *)a = (int)ind - a - 4;	
		}

	} else if( toks.t == TOK_RETURN ) {
		next();
		expr();
		do_return();
		skip(';');
		
	} else if( toks.t == TOK_BREAK ) {
		//printf("in break\n");
		next();
		skip(';');
		block();
	} else if( toks.c == '}' ) {
	} else if( toks.c == '{' ) {
		skip('{');
		while( toks.c != '}' ) {
			block();
		}
		skip('}');

	} else  {

		expr();
		if( toks.c == ';' ) {
			skip(';');
			block();
		}	
	}


}

decl(cls) {

	thisClass = cls;

	if( toks.t == 1003 ) {

		next();
		decl(cls);

	} else if( toks.t == TOK_VAR ) {
		next();
	
		variable *a = safe_alloc_new( &alloc, sizeof(variable *) );

		char *id = toks.id;
		if( cls ) {
			id = mstrcat( cls, "%");
			id = mstrcat(id, toks.id);
		}

		array_set1( &var_stk, id, a );

		next();

		//expr();

		skip(';');
		decl( cls );

	} else if( toks.t == 1006 ) {
		next();
		decl(cls);
	} else if( toks.t == TOK_CLASS ) {

		do_create_class();

		decl(cls);
	
	} else if( toks.t == TOK_IDENT | toks.t == TOK_MAIN ) {
		if( toks.t == TOK_MAIN ) {
			inMain = 1;
		}


		if( thisClass && inMain ) {
			ivar = 0;
			toks.type = 1;
			set_tokv( &toks, ind + 5, cls );


			next();
			do_create_callback_function();
			decl(cls);

		} else {
			ivar = 0;
			toks.type = 1;
			set_tokv( &toks, ind, cls );
			do_create_function(cls);
			decl(cls);
		}


	} else {

		if( ch == EOF ) return;
		block();
	}
}

print_tok() {
	printf("------------------------------------tok : %d ", toks.t);
	if( toks.id ) {
		printf(", c : %s\n", toks.id );
	} else {
		printf(", c1 : %c\n", toks.c );
	}		
}


main(int n, char * t[] )
{

	set_extensions();

	buf = sbuf = safe_alloc_new( &alloc, ALLOC_SIZE);

	ind = prog = mmap(0, ALLOC_SIZE, 7, 0x1002 | MAP_ANON, -1, 0);
	vtoks = malloc( sizeof( tokens * ) * 20 );

	if (!prog) { printf("could not mmap(%d) jit executable memory\n", ALLOC_SIZE); return -1; }

	file = fopen(t[1], "r");

	array_set1( &mt, "if", TOK_IF );
	array_set1( &mt, "else", TOK_ELSE );
	array_set1( &mt, "while", TOK_WHILE );
	array_set1( &mt, "break", TOK_BREAK );
	array_set1( &mt, "return", TOK_RETURN );
	array_set1( &mt, "for", TOK_FOR );
	array_set1( &mt, "define", TOK_DEFINE );
	array_set1( &mt, "main", TOK_MAIN );
	array_set1( &mt, "var", TOK_VAR );
	array_set1( &mt, "new", TOK_NEW );
	array_set1( &mt, "class", TOK_CLASS );
	array_set1( &mt, "in", TOK_IN );
	array_set1( &mt, "func", TOK_FUNC );
	array_set1( &mt, "let", TOK_LET );

	array_init( &sym_stk );
	array_init( &var_stk );
	safe_alloc_init( &alloc );

	inp();
	next();
	decl(0);

	if( t[2] ) {
		if( strcmp(t[2], "-p") == 0 ) {
			print_ind();
		} else {
			FILE *f;
			f = fopen( t[2], "w");
			fwrite((void *)prog, 1, ind - prog, f);
			fclose(f);
		}

	}

	int main = array_get1( &sym_stk, "fn%main");
 	int (*func)() = main;
 	func();

	safe_free( &alloc );
}