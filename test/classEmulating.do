test_a( this ) {
	this.a = "test_a";
}

test_b( this ) {
	this.b = "test_b";
}

main() {
	
	var a = {};
	test_a( a );
	test_b( a );
	print( a );	



	var b = {};
	test_a( b );
	b.a = "test_a2";
	test_b( b );
	b.b = "test_b2";
	print( b );
	
	print( a );

}