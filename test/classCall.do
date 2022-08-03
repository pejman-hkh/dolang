class test {
	construct( a ) {
		print( a );
		
		return this;
	}

	a() {
		print("in a\n");
	}
}

main() {
	new test("test");
}