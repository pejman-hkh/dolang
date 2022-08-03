class test {
	a() {
		print("in a\n");
	}		

	b() {
		print("in b\n");
	}	
}


test(d) {
	d.a();
}

main() {
	
	var a = new test;
	a.b = "Test";
	print("Test");

	print( a );

	test( a );

	a.b();
}