class a {
	construct() {
		return this
	}

	g() {
		print("in g")
	}
}

class b {
	a() {
		this.d = new a()
		print( this )
		return this;
	}

	b() {
		this.d.g();
	}
}

main() {
	var a = new b
	a.a().b()
}