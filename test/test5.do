
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
	

	var b = new b;
	b.a();

}