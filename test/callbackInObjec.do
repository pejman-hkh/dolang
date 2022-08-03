main() {

	var o = { 'a' : { 'b' : func(a) { print(a); } } };
	print(o);

	o.a.b("in function in object");
}