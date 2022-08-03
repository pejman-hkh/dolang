test() {
	return func( a ) {
		print(a);
	};
}


main() {
	var l = test();	
	l("test\n");
}