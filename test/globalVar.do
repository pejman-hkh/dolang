var test_var;

test_b() {
	test_var = "bbb\n";	
}

test_a() {
	test_var = "aaa\n";
}

main() {
	test_var = "test";

	test_a();
	print(test_var);

	test_var = "test1";
	test_b();
	print(test_var);

	var a = "test";
}