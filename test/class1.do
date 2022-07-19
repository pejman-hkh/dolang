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
	a.b().c();
	a.a = "test1\n";
	a.c();
}