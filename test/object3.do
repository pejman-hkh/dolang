main() {

	var o1 = { test : 123 };

	print( o1 );
	var test = 'dd';

	var o2 = { test };
	print( o2 );
	var o21 = { test : 'aaa' };
	print( o21 );

	var o3 = { d : {"a" : 12 } };
	print( o3 );

	var o4 = { o3.d };
	print( o4 );

	var o5 = { o3.d.a : 1 };
	print( o5 );
}