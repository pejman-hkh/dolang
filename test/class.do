class test {
	var a;

	b() {
		print("in b method\n");
	}
	c() {
		print("in c method\n");
	}
}

main() {
	
	var a = new test;

	a.b();
	a.c();
}