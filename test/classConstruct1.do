class test {
	construct( a ) {
		print( this );
		this.g = "test";		
		//this.a();
	}

	a() {
		print("here\n")
		print( this );
	}
}


main() {

	print( test )
	
	var a = new test("test")
	print( a )
	a.a();
}