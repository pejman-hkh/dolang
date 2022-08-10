class test {
	construct( a ) {
		print( this );

		//this.a();
	}

	a() {
		print( this );
	}
}


main() {
	
	var a = new test("test")

	a.a();
}