test( var callback ) {

	return "aaaa";
}

class DB {
	construct() {

	}

	connect() {
		print("db connect\n");
	}
}


class result {

}

class test1 {
	a(b) {
		for( var j = 0; j < 10; j++ ) {
			print("j : "+j+" / ");
			for( var s = 0; s < 10; s++ ) {
				print("s:"+s+",");
			}

			print("\n");
		}
		return "aaa";
	}

	b(a) {
	}

}

class json {

	encode( arr ) {
		//print( arr[0].name );

		this.name = arr[0].name;

		for( var k,v in arr ) {
			print(k);
			print(" : ");
			print(v);
			print("\n");

		}			
	}

	decode( str ) {

	}
}


//var name;

main() {

	var name = "Test\n";
	print(name);

	var arr = {};
	arr.test = "this is test\n";
	print( arr.test );


	var db = new DB;
	db.connect();

	var aa = new test1;
	print( aa.a() );


	var arr2 = {};
	for( var i = 0; i < 10; i++ ) {

		var a = {};
		var b =  test()+" "+i;
		a.id = i;
		a.name = b;

		arr2[i] = a;
	
	}

	print( arr2 );

	var arr1 = [0 : {'name' : 'first'} ];

	var j = new json;
	j.encode( arr1 );

	print( j.name );


	var arr1 = [0 : {'name' : 'second'} ];

	var json1 = new json;
	json1.encode( arr1 );

	print( json1.name );

	print("\n\n\n");

	for( var k,v in arr ) {
		print(k);
		print(" : ");
		print(v);
		print("\n");

	}

	print("test\n");

	var s = "ddd\n";
	print(s);

	print("test1\n");

}