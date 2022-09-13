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

	dovar(k, "re", DOTYPE_STRING );
	dovar(v, re, DOTYPE_RES );
	array_set( ths, k, v );

	return ths;
}

do_regex_exec( variable *ths, variable *subject ) {

	//printf("%s\n", subject->val );
	dovar(k, "re", DOTYPE_STRING );
	variable * re = array_get( ths, k );

	size_t subject_size;//= strlen(subject->val);
	uint32_t options = 0;

	pcre2_match_data *match_data;
	uint32_t ovecsize = 128;

	/* for pcre2_match */
	int rc;
	PCRE2_SIZE* ovector;

	match_data = pcre2_match_data_create(ovecsize, NULL);
	dovar(sl, "lastIndex", DOTYPE_STRING);
	dovar(sind, "index", DOTYPE_STRING);
	dovar(slstr, "str", DOTYPE_STRING);
	
	char *msub = subject->val;

	variable *lastStr = array_get(ths, slstr );
	if( lastStr->type != DOTYPE_UNDEF ) {
		msub = lastStr->val;
	}

	if( ! msub ) {
		dovar(rz, 0, DOTYPE_INT);
		return rz;
	}

	subject_size = strlen(msub);

	rc = pcre2_match(re->val, msub, subject_size, 0, options, match_data, NULL);

	array *r = safe_alloc_new( &alloc, sizeof( array *) );
	array_init(r);
	dovar( arr, r, DOTYPE_ARRAY );

	if(rc == 0) {
		dovar(ml, 0, DOTYPE_INT);
		array_set(ths, sl, ml);
		//fprintf(stderr,"offset vector too small: %d",rc);
	}
	else if(rc > 0)
	{

		variable *lastIndex = array_get(ths, sl );
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

			dovar(ml, lindx+ovector[2*i+1], DOTYPE_INT);
			
			dovar(str1, start+slen, DOTYPE_STRING);
			array_set(ths, slstr, str1);

			array_set(ths, sl, ml);

			dovar(indt, lindx+ovector[2*i], DOTYPE_INT);
			array_set(ths, sind, indt);

			dovar(k, i, DOTYPE_INT);
			dovar(v, s, DOTYPE_STRING);

			array_set( arr, k, v );

		}
	} else {
		dovar(ml, 0, DOTYPE_INT);
		array_set(ths, sl, ml);
	}


	pcre2_match_data_free(match_data);
	//pcre2_code_free(re->val);
	//printf("dddd\n");
	if( r->length == 0 ) {
		dovar(rz, 0, DOTYPE_INT);
		return rz;	
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