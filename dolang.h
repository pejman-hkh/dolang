#ifndef dolang
#define dolang

#include "safe_alloc.h";
safe_alloc alloc;


typedef struct {
	char type;
	void * val;
} variable;

variable *StringClass;
variable *ArrayClass;
variable *ObjectClass;
variable *RegExpClass;


#define dovar(a,b,c) \
variable *a = safe_alloc_new(&alloc, sizeof( variable *) );\
a->val = b;\
a->type = c;

char *file_ext;

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
#define TOK_EXTENDS 19
#define TOK_IMPORT 20
#define TOK_SWITCH 21
#define TOK_TRY 22
#define TOK_CATCH 23
#define TOK_THROW 24
#define TOK_TYPEOF 25
#define TOK_STATIC 26
#define TOK_FUNCTION 27
#define TOK_PACKAGE 28
#define TOK_TRUE 29
#define TOK_FALSE 30
#define TOK_DO 31
#define TOK_REGEX 32


#define DOTYPE_STRING 1
#define DOTYPE_INT 2
#define DOTYPE_ARRAY 3
#define DOTYPE_FUNC 4
#define DOTYPE_CHAR 5
#define DOTYPE_LONG 6
#define DOTYPE_RES 7
#define DOTYPE_CLS 8
#define DOTYPE_UNDEF 9
#define DOTYPE_REGEX 10

#define TOK_IDENT 999
#define TOK_STRING 1000

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
#endif