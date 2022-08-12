do_fopen( variable *ths, variable *fn, variable *s ) {
	FILE *f;
	char *fpath = mstrcat(mainPath,fn->val);
	f = fopen( fpath, s->val );

	if( ! f ) {
		printf("warning: file %s deosn't exists\n", fpath);
	}

	dovar( ret, f, DOTYPE_INT);
	return ret;
}

do_feof( variable *ths, variable *fp ) {
	int a = feof( fp->val );
	dovar( ret, a, DOTYPE_INT );
	return ret;
}

do_fgetc( variable *ths, variable *fp ) {
	char c = fgetc( fp->val );

	char *r = safe_alloc_new( &alloc, sizeof( char *) );
	*r++ = c;
	*r++ = '\0';

	dovar( ret, r-2, DOTYPE_STRING);
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


	dovar( ret, size, DOTYPE_INT);
	return ret;
}


do_fread( variable *ths, variable *fp, variable * size, variable *count ) {
	dovar(ret, "", DOTYPE_STRING );
	if( ! fp->val ) {
	} else {	
		int ss = count->val;
		char *buf = safe_alloc_new( &alloc, sizeof( char ) * ss );
		fread( buf, fp->val, 1, fp->val );

		//dovar( ret, buf, 1);
		ret->val = buf;

	}
	return ret;
}