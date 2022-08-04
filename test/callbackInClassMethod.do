class test {
	a(r, cb) {
		var l = {};
		l.f = func( d ) {
			print(d);
		}

		cb(l);
	}
}

main() {
	
	var a = new test;
	a.a('/', func( b ) {
		b.f("is test\n");
	});

	print("here\n");
}