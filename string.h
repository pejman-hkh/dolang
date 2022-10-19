#ifndef string_h
#define string_h

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char **strings;
	int *types;
	int i;
} string;

string * newstring( char * str );

string * string_init( string *str ) {
	str = malloc( sizeof( string ) );
	str->i = 0;
	str->strings = malloc( sizeof( char **) * (2) );
	str->types = malloc( sizeof( int *) * (2) );
	return str;
}

string * string_cat( string *a, string *b) {
	string *r;
	r = string_init( r );
	string_append1( r, a );
	string_append1( r, b );
	return r;
}

string * string_cat1( string *a, char *b ) {
	return string_cat( a, newstring( b ) );
}

string * string_cat2( char *b, string *a ) {
	return string_cat( newstring( b ), a );
}

void string_append( string *str, char *str1 ) {
	str->strings = realloc( str->strings, sizeof( char * ) * ( str->i + 2 ) );
	str->types = realloc( str->types, sizeof( int *) * ( str->i + 2 ) );
	str->types[str->i] = 1;
	str->strings[str->i++] = str1;
}

void string_append1( string *str, string *str1 ) {
	str->strings = realloc( str->strings, sizeof( char * ) * ( str->i + 2 ) );
	str->types = realloc( str->types, sizeof( int *) * ( str->i + 2 ) );
	str->types[str->i] = 2;
	str->strings[str->i++] = str1;
}

void string_print( string *str ) {
	for( int i = 0; i < str->i; i++ ) {
		if( str->types[i] == 1 ) {
			printf("%s", str->strings[i]);
		} else {
			string_print( str->strings[i] );
		}
	}
}

void string_val1( char *ret, string *str ) {
	for( int i = 0; i < str->i; i++ ) {
		if( str->types[i] == 1 ) {
			strcat( ret, str->strings[i] );
		} else {
			string_val1( ret, str->strings[i] );
		}
	}	
}

void string_line( string *str, string *ret ) {
	for( int i = 0; i < str->i; i++ ) {
		if( str->types[i] == 1 ) {
			if( strcmp(str->strings[i], "") != 0 )
				string_append( ret, str->strings[i]);
		} else {
			string_line( str->strings[i], ret );
		}
	}
}

int string_len( string *str ) {
	int len = 0;
	for( int i = 0; i < str->i; i++ ) {
		if( str->types[i] == 1 ) {
			len += strlen(str->strings[i]);
		} else {
			len += string_len( str->strings[i] );
		}
	}

	return len;
}

char * string_val( string *str ) {

	string *nstr = newstring("");
	string_line( str, nstr );

	int len = string_len( nstr );
	char *ret = malloc( len + 1 );
	memset( ret, 0, len + 1 );

	for( int i = 0; i < nstr->i; i++ ) {
		strcat( ret, nstr->strings[i] );
	}


	return ret;	
}


char *string_index( string *str, int index ) {
	string *ret = newstring("");
	string_line( str, ret );
	int it = 0;
	for( int i = 0; i < ret->i; i++ ) {
		char *str1 = ret->strings[i];
		while( *str1 ) {
			if( index == it ) {
				return str1;
			}
			it++;
			str1++;
		}
	}
	return "";
}

char *string_substr( string *str, int s, int l ) {
	string *nstr = newstring("");
	string_line( str, nstr );

	int it = 0;
	char *ssr, *ssr1;
	ssr = ssr1 = malloc( sizeof( char * ) * ( l+1 ) );
	for( int i = 0; i < nstr->i; i++ ) {
		char *sstr = nstr->strings[i];
		while( *sstr ) {
			if( it >= s ) {
				*ssr++ = *sstr;
			}

			if( it == s + l - 1 ) {
				*ssr = '\0';
				return ssr1;
			}

			sstr++;
			it++;
		}
	}
	return "";
}

typedef struct {
	int i;
	int is;
} stringIter;

char string_iter( string *str, stringIter *it ) {

	if( it->i == str->i ) {
		return 0;
	}

	char *s = str->strings[it->i];
	char p = s[it->is];

	if( p == '\0' ) {

		if( it->i < str->i-1 ) {
			s = str->strings[++it->i];
			it->is = 0;

		}
	}

	return s[it->is++];
}

int string_cmp( string *str1, string *str2 ) {

	string *lstr1 = newstring("");
	string_line( str1, lstr1 );	

	string *lstr2 = newstring("");
	string_line( str2, lstr2 );

	if( string_len( lstr1 ) != string_len( lstr2 ) ) {
		return -2;
	}
	stringIter si1;
	si1.i = 0;
	si1.is = 0;

	stringIter si2;
	si2.i = 0;
	si2.is = 0;


	int j = 1;
	while( 1 ) {
		char a = string_iter( lstr1, &si1 );
		char b = string_iter( lstr2, &si2 );
	

		if( a != b ) {
			return -1;
		}

		if( ! a ) {
			break;
		}
	}

/*	for( int i = 0 ; i < ret->i; i++ ) {
		//char *sstr = ret->
		if( strcmp( ret->strings[i], ret1->strings[i]) != 0 )
			return -1;
	}*/

	return 0;
}


int string_cmp1( string *str1, char *str ) {
	return string_cmp( str1, newstring( str ) );
}

int string_cmp2( char *str, string *str1 ) {
	return string_cmp( newstring( str ), str1 );
}

string * newstring( char * str ) {
    string *str1;
    str1 = string_init( str1 );
    if( strcmp(str, "") != 0 )
    	string_append( str1, str);
    return str1;
}

#endif