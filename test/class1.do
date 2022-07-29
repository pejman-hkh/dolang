class test {
	a() {
		this.a = "this is a";
	}

	b() {
		this.b = "this is b";
	}
}

main() {
	
	var a = new test;
	a.a().b();
	print( a );

}