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

	a.prototype.a = func() {
		this.b();
		print("here\n");
	};

	a.a();
}