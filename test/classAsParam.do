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


test1(d) {
	d.a();
}

main() {
	
	var a = new test;
	print( a );
	a.a();

	test1( a );

	a.b();
}