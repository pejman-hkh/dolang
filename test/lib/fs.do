class fs {

	construct(fname) {
		this.fname = fname;
		this.fp = fopen( fname, "r" );

		return this;
	}

	read() {
		var buf = "";

		var size = fsize( this.fp );

		//var buf = fread( this.fp, 1, size );

		while(  ! feof( this.fp ) ) {
			buf = buf + fgetc( this.fp );

		}


		return buf;
	}
}