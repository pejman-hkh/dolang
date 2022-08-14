#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/mman.h>
#include <dlfcn.h>
#include <dirent.h>
#include "signal.h"
#include "dolang.h"

#include "safe_alloc.h"
safe_alloc alloc;
#include "array.h"
#define ALLOC_SIZE 99999999

char ch;
FILE *mainFile;
FILE *file;
char *buf;
char *sbuf;
char * mainPath;

//int vars;

char *prog;
char *ind;
char *ind_main;


int indvar;
int ivar;
int type;
array ext;

char *thisClass;

int line = 0;

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
array let_stk;
array ind_fns;
//array var_type;
//array var_ref;
tokens ** vtoks;


int inMain = 0;

#include "do-x86.h";
#include "ext.h";

inp() {
	ch = fgetc( file );

	//printf("%c", ch);
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


skip( s ) {
	if( toks.c == s ) {
		next();
	} else {
		printf("Expected %c in line : %d \n", s, line );
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

	if( isspace(ch) && ch != 0x0a ) {
		while( isspace( ch ) && ch != 0x0a ) {
			inp();
		}
	}

	if( ch == EOF ) {
		toks.t = 2023;
		inp();
		//print_tok();
		//exit(0);
	} else if( ch == 0x0a ) {
		line++;
		toks.t = 2022;
		inp();
	} else if( ch == '#' ) {
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
			int mline = line;
			while( ch != '\'' ) {
				getq();
				*buf++ = ch;
				inp();
				if( ch == EOF ) {
					printf("syntax error! line : %d\n", mline);
					exit(0);
				}				
			}
			*buf++ = 0;
			inp();
		} else if( ch == '"' ) {
			toks.t = TOK_STRING;
			inp();
			toks.id = buf;
			int mline = line;
			while( ch != '"' ) {
				getq();
				*buf++ = ch;
				inp();
				if( ch == EOF ) {
					printf("syntax error! line : %d\n", mline);
					exit(0);
				}
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

				if( ch == '=' ) {
					toks.l = 11;
					toks.t = 2026;
					toks.c = ch;
					inp();					
				}

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

				if( ch == '=' ) {
					toks.l = 11;
					toks.t = 2027;
					toks.c = ch;
					inp();					
				}

			} else if( toks.c == '%' ) {
				toks.l = 1;
				toks.t = 2001;

				if( ch == '=' ) {
					toks.l = 11;
					toks.t = 2028;
					toks.c = ch;
					inp();					
				}


			} else if( toks.c == '+' ) {
				toks.l = 2;
				toks.t = 2002;
				if( ch == '+' ) {
					toks.l = 11;
					toks.t = 2003;
					toks.c = ch;
					inp();
				}
				if( ch == '=' ) {
					toks.l = 11;
					toks.t = 2024;
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

				if( ch == '=' ) {
					toks.l = 11;
					toks.t = 2025;
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

					if( ch == '=' ) {
						toks.l = 11;
						toks.t = 2032;
						toks.c = ch;
						inp();					
					}

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

					if( ch == '=' ) {
						toks.l = 11;
						toks.t = 2033;
						toks.c = ch;
						inp();			
					}

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

				if( ch == '=' ) {
					toks.l = 11;
					toks.t = 2029;
					toks.c = ch;
					inp();					
				}

			} else if( toks.c == '^' ) {
				toks.l = 7;
				toks.t = 2017;

				if( ch == '=' ) {
					toks.l = 11;
					toks.t = 2031;
					toks.c = ch;
					inp();					
				}

			} else if( toks.c == '|' ) {
				toks.l = 8;
				toks.t = 2018;

				if( ch == '|' ) {
					toks.l = 10;
					toks.t = 2019;
					toks.c = ch;
					inp();
				}

				if( ch == '=' ) {
					toks.l = 11;
					toks.t = 2030;
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

	if( toks.t == TOK_NEW ) {
		
		next();
		char *cls = toks.id;
		next();

		do_call_class( cls );


	} else if( toks.t == TOK_FUNC ) {
		next();
		if( toks.c == '(') {
			int l = ind+5;
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

		dovar( var, toks.id, 1 );
		do_call_string( var );

		next();
		if( toks.c == '.' ) {
			//call string functions or variables
			do_get_val();
			next();
			next();
		}
	//} else if( toks.t == 2022 ) {
		//next();
	} else if( toks.c == '}' ) {
	
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
			//do_create_callback_function();
		} else if( btoks.t == 1 | btoks.t == TOK_VAR  ) {
			if( toks.c == '*' ) {
				next();
			}

			do_create_var( 4 );
	
		} else if( btoks.c == '!' ) {
			unary();
			do_not();
		} else if( btoks.c == '&' ) {
	
			int l = array_get1( &var_stk, toks.id);
			do_call_address( l );

			next();

		} else if( toks.t == TOK_STRING & btoks.c == '+' ) {
			/*printf("ddddddddddd\n");
			exit(0);*/

		} else if( btoks.t == TOK_NEW ) {

			//do_call_class( &btoks, &toks );
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
		} else if( toks.t >= 2024 && toks.t <= 2032 ) {
			char *id = btoks.id;
			int l = array_get1( &var_stk, id);
		
			do_plus_equal(l);

		} else if( toks.c == '=' & toks.l == 0 ) {

			next();

			char *id = btoks.id;
			/*if( thisClass ) {
				id = mstrcat( thisClass, "%");
				id = mstrcat(id, btoks.id);
			}*/

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
				int ind_fn = do_call_function( l, btoks.id );

				if( ind_fn ) {
					array_set1( &ind_fns, ind_fn, btoks.id );
				}

				if( toks.c == '.' ) {
					vars_init();
					ivar = ivar - 4;
					*(int *)indvar = -ivar;
					int ld = ivar;
					do_equal(ld);
					do_dot(ld);
				}
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

				do_dot( l );
			}			
		} else if( toks.c == ';' ) {
		} else {
		
		}

	}

}

do_dot( l ) {
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
				} else if( btoks.c == '&' ) {
					a = do_and_and( a );
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
					//do_not();
				} else if( btoks.t == 2013 ) {
					do_not_equal();
				} else if( btoks.t == 2014 ) {
					do_equal_equal();
				} else if( btoks.t == 2015 ) {
					do_and();
				} else if( btoks.t == 2016 ) {
					//do_and_and();
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
				int d = a;
				a = do_or_or( a );
				do_patch_or_or( a, d );
			} else if( btoks.c == '&' ) {
				int d = a;
				a = do_and_and( a );
				do_patch_and_and( a, d );				
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

	} else if( toks.t == 2022 ) {
		next();
		block();
	} else if( toks.t == TOK_VAR ) {
		next();
		do_create_var(4);
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
		//skip(';');
		
	} else if( toks.t == TOK_BREAK ) {
		//printf("in break\n");
		next();
		//skip(';');
		block();
/*	} else if( toks.c == '}' ) {
		print_tok();
*/
	} else if( toks.c == '{' ) {

		skip('{');
		while( toks.c != '}' ) {
			int mline = line;
			block();

			if( toks.t == 2023 ) {
				printf("syntax error ! line : %d\n", mline );
				exit(0);
			}

		}
		skip('}');

	} else  {

		expr();

		if( toks.t == 2022 ) {
			next();
		}

		if( toks.c == ';' ) {
			skip(';');
		}	
	}


}

decl(cls) {

	thisClass = cls;

	if( toks.t == 0 ) {
		next();
		decl(cls);
	} else if( toks.t == 2022 ) {
		next();
		decl(cls);
	} else if( toks.t == 1003 ) {

		next();
		decl(cls);

	} else if( toks.t == TOK_IMPORT ) {

		next();

		char *fn;
		fn = toks.id;
		next();


		while( toks.c == '.' ) {
			next();
			fn = mstrcat(fn, "/");
			fn = mstrcat(fn, toks.id );

		}
		
		fn = mstrcat(mainPath, fn);
		fn = mstrcat( fn ,".do");


		FILE *f1;
		f1 = fopen( fn, "r");
		if( f1 == NULL ) {
			printf("file not exists !\n");
			exit(0);
		}

		do_run( f1 );
		fclose(f1);

		file = mainFile;
		inp();
		next();


		//file = mainFile;
		decl(cls);

	} else if( toks.t == TOK_VAR ) {
		next();
	
		variable *a = safe_alloc_new( &alloc, sizeof(variable *) );

		char *id = toks.id;
	/*	if( cls ) {
			id = mstrcat( cls, "%");
			id = mstrcat(id, toks.id);
		}*/

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

do_run( f ) {
	file = f;
	inp();
	next();
	decl(0);
}

do_get_path(fn) {
	int ls = strrchr(fn, '/');
	if( ls ) {
		mainPath = fn;
		mainPath[ ls - fn ] = '/';
		mainPath[ ls - fn + 1 ] = '\0';
	} else {
		mainPath = "/";
	}

}

typedef array *(*loadfn)();

scan_ext() {
	struct dirent **namelist;
	int n;

	n = scandir("./ext/", &namelist, NULL, alphasort);
	if (n < 0)
	   perror("scandir");
	else {
		while (n--) {
			char *dir = namelist[n]->d_name;
			if( strcmp( dir, "." ) != 0 && strcmp( dir, ".." ) != 0 ) {

				char *ld = mstrcat("./ext/", dir );
				ld = mstrcat(ld, "/" );
				ld = mstrcat(ld, dir );
				ld = mstrcat(ld, ".so" );

				void *myso = dlopen( ld, RTLD_NOW );
				if ( myso ) {
					loadfn load = (loadfn)dlsym(myso, "load" );
					array *arr = load();
					for( int i = 0; i < arr->length; i++ ) {
						array_set1( &ext, arr->key[i], arr->value[i]);
					}
					array_free( arr );
					free( arr );
				}
			}
			free(namelist[n]);
		}
		free(namelist);
	}	
}


main(int n, char * t[] )
{

	//init_signal();
	set_extensions();
	scan_ext();

	buf = sbuf = safe_alloc_new( &alloc, ALLOC_SIZE);
	ind = prog = mmap(0, ALLOC_SIZE, 7, 0x1002 | MAP_ANON, -1, 0);
	vtoks = malloc( sizeof( tokens * ) * 20 );

	if (!prog) { printf("could not mmap(%d) jit executable memory\n", ALLOC_SIZE); return -1; }

	

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
	array_set1( &mt, "extends", TOK_EXTENDS );
	array_set1( &mt, "import", TOK_IMPORT );

	array_init( &sym_stk );
	array_init( &var_stk );
	array_init( &ind_fns );
	safe_alloc_init( &alloc );


	mainFile = fopen(t[1], "r");
	do_get_path(t[1]);

	if( t[2] ) {

		if( strcmp(t[2], "-t") == 0 ) {

			file = mainFile;
			inp();

			while( ch != EOF  ) {
				print_tok();
				next();
			}

			exit(0);
		}
	}

	do_run( mainFile );
	fclose(mainFile);

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


	//call functions after defination
	for( int i = 0; i < ind_fns.length; i++ ) {
		char *t;
		t = mstrcat("fn%", ind_fns.value[i]);
		int l = array_get1( &sym_stk, t );
		int mind = ind_fns.key[i];
		int n = l - (mind -1) - 5;
	

		*(int *)mind = n;

	}

	int main = array_get1( &sym_stk, "fn%main");
	if( main ) {
 		int (*func)() = main;
 		func();
	} else {
		printf("main function not exists !\n");
	}

	safe_free( &alloc );
}