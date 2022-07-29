class test {
	a() {
		this.a = "this is a";
		print( this.c );
	}

	b() {
		this.b = "this is b";
		print( this.c );
	}
}

main() {
	
	var a = new test;
	a.c = "this is c\n";

	a.a().b();
	print( a );

}