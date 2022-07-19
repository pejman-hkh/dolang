class test {
	var a;

	b() {
		this.a = "this is test\n";
	}

	c() {
		print( this.a );
	}

}

main() {
	
	var a = new test;
	a.b();
	a.a = "test1\n";
	a.c();	


	var b = new test;
	b.b();

	b.c();

}