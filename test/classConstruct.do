class test {
	construct( a ) {
		print("in construct " + a +"\n" );
		print("here\n")

		this.a();
	}

	a() {

		print("in a\n");
		return this;
	}

	b() {
		print("in b\n");
	}
}


main() {
	var a = new test("test");
	a.a().b();


	print( a );
}