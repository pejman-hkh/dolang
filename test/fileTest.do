main() {
	var fp = fopen( "test.txt", "r" );

	var buf = "";
	
	while(  ! feof( fp ) ) {
		buf = buf + fgetc( fp );

	}
	
	print( buf );

}