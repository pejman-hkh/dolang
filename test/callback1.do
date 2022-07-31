main() {

	var l = func( a ) {
		print("in callback\n");
		print( a );
	}

	l("test");


	/*var d = 'a';

	var a = { d : func(a) { return a+1; } };


	a.a();*/

}