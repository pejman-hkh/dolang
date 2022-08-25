class a {
	construct() {
		return this
	}

	g() {
		print("in g\n")
		print( this )
		this.l()
	}

	l() {
		print("in l\n")
	}
}

class b {
	a() {
		this.d = new a()
		return this;
	}

	b() {
		this.d.g();
	}
}

main() {
	var o = new b
	o.a().b()
}