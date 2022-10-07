do_fopen( variable *ths, variable *fn, variable *s ) {
	FILE *f;
	char *fpath = mstrcat(mainPath,fn->val);
	f = fopen( fpath, s->val );

	if( ! f ) {
		printf("warning: file %s deosn't exists\n", fpath);
	}

	variable * ret = donvar( f, DOTYPE_INT );
	return ret;
}

do_feof( variable *ths, variable *fp ) {
	int a = feof( fp->val );
	variable * ret = donvar( a, DOTYPE_INT );
	return ret;
}

do_fgetc( variable *ths, variable *fp ) {
	char c = fgetc( fp->val );
	char *r;
	char *a;
	r = a = safe_alloc_new( &alloc, 2 );
	if( c == EOF ) {
		c = '\0';
	}

	*r++ = c;
	*r++ = '\0';
	
	variable * ret = donvar( a, DOTYPE_STRING );

	return ret;
}

do_fsize( variable *ths, variable *fp ) {
		int size = 0;
	if( ! fp->val ) {
		size = 0;
	} else {	
		fseek (fp->val , 0 , SEEK_END);
		size = ftell (fp->val);
		rewind (fp->val);
	}


	variable * ret = donvar( size, DOTYPE_INT );
	return ret;
}


do_fread( variable *ths, variable *fp, variable * size, variable *count ) {
	variable * ret = donvar( "", DOTYPE_STRING );
	if( ! fp->val ) {
	} else {	
		int ss = count->val;
		char *buf = safe_alloc_new( &alloc, sizeof( char ) * ss );
		fread( buf, fp->val, 1, fp->val );

		//variable * ret = donvar( buf, 1 );
		ret->val = buf;

	}
	return ret;
}