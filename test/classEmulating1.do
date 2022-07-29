test_a( this ) {
	this.a = "test_a";
}

main() {
	
	var a = {};
	test_a( a );
	print( a );	
}