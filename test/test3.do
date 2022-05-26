test() {

	var arr = [0 : ['name' : 'second'], 1 : ['name' : 'test1'] ];

	for( var k,v in arr ) {
		print(k);
		print(" : ");
		print(v);
		print("\n");

	}
}


main() {
	test();
}