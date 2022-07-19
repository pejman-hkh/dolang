class test {
	var a;

	b() {
		this.a = "this is test";
		print( this.a );
	}

}

main() {
	
	var a = new test;
	a.b();

}