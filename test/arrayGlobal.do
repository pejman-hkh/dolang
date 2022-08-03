
var a;

test() {
	print(a);
}

main() {
	a = [];
	a['t'] = 'test1';
	a['2'] = 'test2';
	a['3'] = 'test3';
	test();
}