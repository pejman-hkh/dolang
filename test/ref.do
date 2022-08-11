test( this ) {
	this.d = 321;
}

main() {
	var this = {};
	test( this );
	print( this );
}