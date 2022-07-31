class test {
	c() {
		this.s = "ddd";
	}

	b() {
		print("test ...");
		this.d = 123;
	}

	a() {
		this.b();
	}

}

main() {
	var a = new test;
	a.a().c();
	print( a );
}