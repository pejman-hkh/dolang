
class a {
	a() {
		print("in a\n");
	}
}

class b extends a {
	b() {

	}
}


main() {
	var d = new b;
	d.a();

	print( d );
}