main() {


	var s = "test s";
	var l = func( a ) {
	
		print("in callback\n");
		print( a );
	};

	l("test\n");

	var add = func( a , b ) {
		return a+b;
	};


	print( "sum of 2 and 4 is : "+add( 2, 4)+"\n" );

}