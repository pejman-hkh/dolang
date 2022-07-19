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
	
	var n = new test;
	n.b();
	n.a = "test1\n";
	n.c();
}