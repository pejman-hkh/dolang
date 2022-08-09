
main() {

	var d =  { a : 'test'}
	d.b = func() {
		print("test")
	}

	d.class = "test";
	d.b();

	print( d );
}