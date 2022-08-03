class test {
	a() {
		print("in a\n");
		return this;
	}

	b() {

		print("in b\n");
		return this;
	}
}

test() {
	var a = new test;
	return a;	
}

main() {
	test().a().b();
}