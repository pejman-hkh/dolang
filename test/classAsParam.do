class test {
	a() {
		//this.d = "ddd";

		print("in a\n");
		return this;
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
	print( a );
	a.a();

	test( a );

	a.b();
}