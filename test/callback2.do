test( callback ) {
	callback(1,2);
}

main() {
	test(func( a, b ) {
		print(a+b);
	});
}