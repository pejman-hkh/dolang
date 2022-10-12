#include <stdio.h>
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

#include "dolang.h"
#include "safe_alloc.h"
#include "array.h"
#include "fn.h"



do_regex( variable *ths, variable *pattern, variable *ex ) {

	pcre2_code *re;
	PCRE2_SIZE erroffset;
	int errcode;
	PCRE2_UCHAR8 buffer[128];


	size_t pattern_size = strlen(pattern->val);
	uint32_t options = 0;

	char *a = pattern->val;
	//a = mstrcat("/", pattern->val);
	//a = mstrcat(a, "/");
	//a = mstrcat(a, ex->val );

	//printf("%s\n", a);
	//exit(0);

	re = pcre2_compile(a, pattern_size, options, &errcode, &erroffset, NULL);
	if (re == NULL)
	{
		pcre2_get_error_message(errcode, buffer, 120);
		//fprintf(stderr,"%d\t%s\n", errcode, buffer);
		return 1;
	}

	array_set( ths, dostring("re"), donvar( re, DOTYPE_RES ) );

	return ths;
}

do_regex_exec( variable *ths, variable *subject ) {

	//printf("%s\n", subject->val );

	variable * re = array_get( ths, dostring("re") );

	size_t subject_size;//= strlen(subject->val);
	uint32_t options = 0;

	pcre2_match_data *match_data;
	uint32_t ovecsize = 128;

	/* for pcre2_match */
	int rc;
	PCRE2_SIZE* ovector;

	match_data = pcre2_match_data_create(ovecsize, NULL);

	char *msub = subject->val;

	variable *lastStr = array_get(ths, dostring("str") );
	if( lastStr->type != DOTYPE_UNDEF ) {
		msub = lastStr->val;
	}

	if( ! msub ) {

		return doint(0);
	}

	subject_size = strlen(msub);

	rc = pcre2_match(re->val, msub, subject_size, 0, options, match_data, NULL);

	array *r = safe_alloc_new( &alloc, sizeof( array *) );
	array_init(r);
	variable * arr = donvar( r, DOTYPE_ARRAY );

	if(rc == 0) {

		array_set(ths, dostring("lastIndex"), doint(0));
		//fprintf(stderr,"offset vector too small: %d",rc);
	}
	else if(rc > 0)
	{

		variable *lastIndex = array_get(ths, dostring("lastIndex") );
		int lindx = 0;
		if( lastIndex->type != DOTYPE_UNDEF ) {
			lindx = (int)lastIndex->val;
		}

		//printf("%d\n", rc);
		ovector = pcre2_get_ovector_pointer(match_data);
		PCRE2_SIZE i;
		for(i = 0; i < rc; i++) {

			PCRE2_SPTR start = msub + ovector[2*i];
			PCRE2_SIZE slen = ovector[2*i+1] - ovector[2*i];
			
			char *s = safe_alloc_new( &alloc, slen );
			memcpy(s,start, slen);
			s[slen] = '\0';

			variable * str1 = dostring(start+slen);
			array_set(ths, dostring("str"), str1);

			array_set(ths, dostring("lastIndex"), doint(lindx+ovector[2*i+1]));

			variable * indt = doint(lindx+ovector[2*i]);
			array_set(ths, dostring("index"), indt);

			array_set( arr, doint(i), dostring(s) );

		}
	} else {

		array_set(ths, dostring("lastIndex"), doint(0));
	}


	pcre2_match_data_free(match_data);
	//pcre2_code_free(re->val);
	//printf("dddd\n");
	if( r->length == 0 ) {
		return doint(0);	
	}

	return arr;
}


extern load() {
    array *arr = malloc( sizeof( array *) );
    array_init( arr );
    array_set1( arr, "RegExp%fn%construct", &do_regex);
    array_set1( arr, "RegExp%fn%exec", &do_regex_exec);

    return arr;
}