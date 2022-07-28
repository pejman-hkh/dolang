
test( a ) {
	
	a.d = 321;
}

main() {
	var a = { 'd' : 123 };
	test( a );
	print( a );
}