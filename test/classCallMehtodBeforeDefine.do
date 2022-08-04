class test {
	a() {
		this.b();
	}

	b() {
		print("in b\n");
	}
}

main() {
	var a = new test;
	a.a();
}