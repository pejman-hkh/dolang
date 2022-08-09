do_fopen( variable *ths, variable *fn, variable *s ) {
	FILE *f;
	f = fopen( fn->val, s->val );

	dovar( ret, f, 2);
	return ret;
}

do_feof( variable *ths, variable *fp ) {
	int a = !feof( fp->val );
	dovar( ret, a, 2 );
	return ret;
}

do_fgetc( variable *ths, variable *fp ) {
	char c = fgetc( fp->val );

	char *r = safe_alloc_new( &alloc, sizeof( char *) );
	*r++ = c;
	*r++ = '\0';

	dovar( ret, r-2, 1);
	return ret;
}

do_fsize( variable *ths, variable *fp ) {

	fseek (fp->val , 0 , SEEK_END);
	int size = ftell (fp->val);
	rewind (fp->val);

	dovar( ret, size, 2);
	return ret;
}


do_fread( variable *ths, variable *fp, variable * size, variable *count ) {
	int ss = count->val;
	char *buf = safe_alloc_new( &alloc, sizeof( char ) * ss );
	fread( buf, fp->val, 1, fp->val );

	dovar( ret, buf, 1);
	return ret;
}