class test {
	b() {
		print("in b ... \n");
	}

	a() {
		this.b();
		print( this );
	}
}


main() {
	
	var a = new test;
	a.c = "Test";

	a.a = func() {
		print("here\n");
	}

	a.a();
}