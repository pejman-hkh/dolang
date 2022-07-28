test_a( this ) {
	this.a = "test_a";
}

test_b( this ) {
	this.b = "test_b";
}

main() {
	
	var this1 = {};
	test_a( this1 );
	test_b( this1 );
	print( this1 );	



	var this2 = {};
	test_a( this2 );
	this2.a = "test_a2";
	test_b( this2 );
	this2.b = "test_b2";
	print( this2 );
	
	print( this1 );

}