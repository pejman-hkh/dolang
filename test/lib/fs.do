class fs {

	construct(fname) {
		this.fname = fname;
		this.fp = fopen( fname, "r" );

		return this;
	}

	read() {
		var buf = "";

		var size = fsize( this.fp );

		while(  ! feof( this.fp ) ) {
			buf += fgetc( this.fp );
		}

		return buf;
	}
}