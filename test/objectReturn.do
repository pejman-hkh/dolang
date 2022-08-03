test() {
	var o = { 'a' : { 'b' : func(a) { print(a); } } };
	return o;
}

main() {
	var b = test();

	b.a.b("test\n");
}