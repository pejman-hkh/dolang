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

test1() {
	var a = new test;
	return a;	
}

main() {
	test1().a().b();
}