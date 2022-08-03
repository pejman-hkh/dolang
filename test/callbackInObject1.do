main() {


	var o = {};
	o.a = func(a) {
		print(a);
	}
	
	print( o );

	o.a("in function in object1\n");

	o.a = func( a ) {
		print(a + "+++++\n");
	}

	o.a("in function in object1");
}